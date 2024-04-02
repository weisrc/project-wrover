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

typedef Result<char, ReadError> read_result_t;
typedef Result<bool, WriteError> write_result_t;
typedef Promise<read_result_t> read_promise_t;
typedef Promise<write_result_t> write_promise_t;

class AsyncStream
{
private:
    Stream &stream;
    unsigned long timeout;
    char ackValue;

    std::list<std::function<void()>> taskQueue;

    std::shared_ptr<read_promise_t> readPromise;
    std::shared_ptr<write_promise_t> writePromise;

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

        if (writePromise != nullptr)
        {
            LOG_DEBUG("TX: Got ack for " + String(lastWriteValue) + " (" + String((int)lastWriteValue) + ")");
            writePromise->resolve(0);
            writePromise.reset();
            LOG_DEBUG("writePromise: " + String(writePromise != nullptr));
        }
        else if (readPromise != nullptr)
        {
            LOG_DEBUG("RX: " + String(data) + " (" + String((int)data) + ")");
            put(ackValue);
            readPromise->resolve(read_result_t(data));
            readPromise.reset();
        }
    }

    void updateWhenNotAvailable()
    {
        unsigned long now = millis();
        if (now - startTime > timeout)
        {
            if (writePromise != nullptr)
            {
                LOG_WARN("TX: Timeout, resending " + String(lastWriteValue) + " (" + String((int)lastWriteValue) + ")");
                put(lastWriteValue);
            }
            else if (readPromise != nullptr)
            {
                LOG_WARN("RX: Timeout, aborting read");
                readPromise->resolve(read_result_t::err(TIMEOUT)); // Provide the missing argument list for the class template "Result"
                readPromise.reset();
            }
            startTime = now;
            return;
        }
    }

public:
    AsyncStream(Stream &stream, unsigned long timeout = 50, char ackValue = 0) : stream(stream), timeout(timeout), ackValue(ackValue) {
    }

    int available()
    {
        return stream.available();
    }

    std::shared_ptr<read_promise_t> read()
    {
        auto promise = std::make_shared<read_promise_t>();

        addTask([this, promise]()
                {   readPromise = promise; 
                    startTime = millis(); });

        return promise;
    }

    std::shared_ptr<write_promise_t> write(char data)
    {
        auto promise = std::make_shared<write_promise_t>();

        addTask([this, promise, data]()
                {
                    lastWriteValue = data;
                    put(data);
                    writePromise = promise;
                    startTime = millis(); });

        return promise;
    }

    std::shared_ptr<write_promise_t> write(String data)
    {
        std::shared_ptr<write_promise_t> promise;

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