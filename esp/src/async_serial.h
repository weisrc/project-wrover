#pragma once

#include <Arduino.h>
#include "logger.h"
#include "promise.h"

#define WRITE_BUFFER_SIZE 256

enum AsyncSerialError
{
    QUEUE_FULL,
    TIMEOUT
};

typedef Result<char, AsyncSerialError> ReadResult;
typedef Result<bool, AsyncSerialError> WriteResult;
typedef Promise<ReadResult> ReadPromise;
typedef Promise<WriteResult> WritePromise;

class AsyncSerial
{
private:
    UARTBase &stream;
    unsigned long timeout;
    char ackValue;
    bool oddParity = false;
    size_t maxTaskQueueSize = 100;
    bool isResolving = false;

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

    bool addTask(std::function<void()> task)
    {
        if (taskQueue.size() >= maxTaskQueueSize)
        {
            LOG_WARN("Async task queue full");
            return false;
        }
        /*
         * When inside a then closure (.i.e. resolving a promise), we want to prioritize the task defined in the closure
         * serial.write('A').finally([] { serial.write('B'); });
         * serial.write('C');
         * In this example, 'B' should be sent before 'C'
         */
        if (isResolving)
            taskQueue.push_front(task);
        else
            taskQueue.push_back(task);
        return true;
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

            isResolving = true;
            writePromise->resolve(0);
            isResolving = false;
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
            isResolving = true;
            readPromise->resolve(ReadResult(data));
            isResolving = false;
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
            isResolving = true;
            readPromise->resolve(ReadResult::fail(AsyncSerialError::TIMEOUT));
            isResolving = false;
            readPromise.reset();
        }
        startTime = now;
    }

public:
    AsyncSerial(UARTBase &stream, unsigned long timeout = 50,
                char ackValue = 0, bool oddParity = true, size_t maxTaskQueueSize = 100) : stream(stream), timeout(timeout),
                                                                                           ackValue(ackValue), oddParity(oddParity),
                                                                                           maxTaskQueueSize(maxTaskQueueSize)
    {
    }

    int available()
    {
        return stream.available();
    }

    std::shared_ptr<ReadPromise> read()
    {
        auto promise = std::make_shared<ReadPromise>();

        bool ok = addTask([&, promise]()
                          {   readPromise = promise; 
                    startTime = millis(); });

        if (!ok)
            promise->resolve(ReadResult::fail(AsyncSerialError::QUEUE_FULL));

        return promise;
    }

    std::shared_ptr<WritePromise> write(char data)
    {
        auto promise = std::make_shared<WritePromise>();

        bool ok = addTask([&, data, promise]()
                          {
                    lastWriteValue = data;
                    put(data);
                    writePromise = promise;
                    startTime = millis(); });

        if (!ok)
            promise->resolve(WriteResult::fail(AsyncSerialError::QUEUE_FULL));

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