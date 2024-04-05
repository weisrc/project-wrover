#pragma once

#include <Arduino.h>
#include "logger.h"
#include "promise.h"

#define WRITE_BUFFER_SIZE 256

enum ReadError
{
    TIMEOUT = 0
};

enum WriteError
{
    WRITE_TIMEOUT = 0
};

typedef Result<char, ReadError> ReadResult;
typedef Result<bool, WriteError> WriteResult;
typedef Promise<ReadResult> ReadPromise;
typedef Promise<WriteResult> WritePromise;

class AsyncStream
{
private:
    UARTBase &stream;
    unsigned long timeout;
    char ackValue;
    bool oddParity = false;

    std::list<std::function<void()>> taskQueue;

    std::shared_ptr<ReadPromise> readPromise;
    std::shared_ptr<WritePromise> writePromise;

    char lastWriteValue = 0;
    unsigned long startTime = 0;

    void put(char data)
    {
        stream.write(data);
        LOG_DEBUG("TX: " + String(data) + " (" + String((int)data) + ")");
    }

    void addTask(std::function<void()> task)
    {
        taskQueue.push_back(task);
    }

    void tryProcessNextTask()
    {
        if (taskQueue.empty())
            return;

        if (readPromise == nullptr && writePromise == nullptr)
        {
            auto task = taskQueue.front();
            taskQueue.pop_front();
            task();
        }
    }

    void updateWhenAvailable()
    {
        char data = stream.read();
        bool targetParity = oddParity ? stream.parityOdd(data) : stream.parityEven(data);
        bool parityOk = stream.readParity() == targetParity;

        if (writePromise != nullptr)
        {
            if (data != lastWriteValue)
            {
                LOG_WARN("TX ack mismatch: " + String((int)data) + " != " + String((int)lastWriteValue) + " parityOk: " + String(parityOk));
            }
            else
            {
                LOG_DEBUG("TX: Got ack for " + String(lastWriteValue) + " (" + String((int)lastWriteValue) + ")");
            }

            writePromise->resolve(0);
            writePromise.reset();
            LOG_DEBUG("writePromise: " + String(writePromise != nullptr));
            return;
        }
        if (!parityOk)
        {
            LOG_WARN("RX: Parity error");
            return;
        }

        if (readPromise != nullptr)
        {
            LOG_DEBUG("RX: " + String(data) + " (" + String((int)data) + ")");
            put(ackValue);
            readPromise->resolve(ReadResult(data));
            readPromise.reset();
            return;
        }
        else
        {
            LOG_WARN("Unexpected RX: " + String(data) + " (" + String((int)data) + ")");
            put(ackValue);
        }
    }

    void updateWhenNotAvailable()
    {
        unsigned long now = millis();
        if (now - startTime < timeout)
            return;

        if (writePromise != nullptr)
        {
            LOG_WARN("TX: Timeout, resending " + String(lastWriteValue) + " (" + String((int)lastWriteValue) + ")");
            put(lastWriteValue);
        }
        else if (readPromise != nullptr)
        {
            LOG_WARN("RX: Timeout, aborting read");
            readPromise->resolve(ReadResult::fail(TIMEOUT)); // Provide the missing argument list for the class template "Result"
            readPromise.reset();
        }
        startTime = now;
    }

public:
    AsyncStream(UARTBase &stream, unsigned long timeout = 50,
                char ackValue = 0, bool oddParity = true) : stream(stream), timeout(timeout),
                                                     ackValue(ackValue), oddParity(oddParity)
    {
    }

    int available()
    {
        return stream.available();
    }

    std::shared_ptr<ReadPromise> read()
    {
        auto promise = std::make_shared<ReadPromise>();

        addTask([&, promise]()
                {   readPromise = promise; 
                    startTime = millis(); });

        return promise;
    }

    std::shared_ptr<WritePromise> write(char data)
    {
        auto promise = std::make_shared<WritePromise>();

        addTask([&, data, promise]()
                {
                    lastWriteValue = data;
                    put(data);
                    writePromise = promise;
                    startTime = millis(); });

        return promise;
    }

    std::shared_ptr<WritePromise> write(String data)
    {
        std::shared_ptr<WritePromise> promise;

        for (size_t i = 0; i < data.length(); i++)
        {
            char c = data[i];
            promise = write(c);
        }

        return promise;
    }

    void update()
    {
        tryProcessNextTask();

        if (available())
            updateWhenAvailable();
        else
            updateWhenNotAvailable();
    }
};