/**
 * @author Wei
 * Acknowledgement based async serial communication using Promises and Task Queue
 * This also has parity checking
 */

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
  size_t maxTaskQueueSize = 256;
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

  /**
   * Add a task to the task queue
   * @param task callback function
   * @return false if the queue is full
   */
  bool addTask(std::function<void()> task)
  {
    if (taskQueue.size() >= maxTaskQueueSize)
    {
      LOG_DEBUG("Async task queue full");
      return false;
    }
    /*
     * When inside a then closure (.i.e. resolving a promise), we want to prioritize the task
     * defined in the closure serial.write('A').finally([] { serial.write('B'); });
     * serial.write('C');
     * In this example, 'B' should be sent before 'C'
     */
    if (isResolving)
      taskQueue.push_front(task);
    else
      taskQueue.push_back(task);
    return true;
  }

  /**
   * Process the next task in the queue if available
   */
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

  /**
   * Update the state when data is available
   */
  void updateWhenAvailable()
  {
    char data = stream.read();
    bool targetParity = oddParity ? stream.parityOdd(data) : stream.parityEven(data);
    bool isParityOk = stream.readParity() == targetParity;

    if (writePromise != nullptr)  // handle ack if writing
    {
      if (data != lastWriteValue)  // The ack should preferably be an echo of the data sent.
      {
        LOG_WARN("TX ack mismatch: " + String((int)data) + " != " + String((int)lastWriteValue) +
                 " isParityOk: " + String(isParityOk));
      }
      else
      {
        LOG_DEBUG("TX: Got ack for " + String(lastWriteValue) + " (" +
                  String((int)lastWriteValue) + ")");
      }

      isResolving = true;
      writePromise->resolve(0);
      isResolving = false;
      writePromise.reset();
      LOG_DEBUG("writePromise: " + String(writePromise != nullptr));
      return;
    }
    if (!isParityOk)
    {
      LOG_WARN("RX: Parity error");
      return;
    }

    if (readPromise != nullptr)  // handle read if reading
    {
      LOG_DEBUG("RX: " + String(data) + " (" + String((int)data) + ")");
      put(ackValue);
      isResolving = true;
      readPromise->resolve(ReadResult(data));
      isResolving = false;
      readPromise.reset();
      return;
    }
    else  // handle unexpected data
    {
      LOG_WARN("Unexpected RX: " + String(data) + " (" + String((int)data) + ")");
      put(ackValue);
    }
  }

  void updateWhenNotAvailable()
  {
    unsigned long now = millis();
    if (now - startTime < timeout)  // wait for timeout before resending
      return;

    if (writePromise != nullptr)
    {
      LOG_WARN("TX: Timeout, resending " + String(lastWriteValue) + " (" +
               String((int)lastWriteValue) + ")");
      put(lastWriteValue);
    }
    else if (readPromise != nullptr)  // if expected data is not received
    {
      LOG_WARN("RX: Timeout, aborting read");
      isResolving = true;
      readPromise->resolve(ReadResult::fail(AsyncSerialError::TIMEOUT));
      isResolving = false;
      readPromise.reset();
    }
    startTime = now;  // reset the timeout timer
  }

public:
  AsyncSerial(UARTBase &stream, unsigned long timeout = 100, char ackValue = 0,
              bool oddParity = true, size_t maxTaskQueueSize = 100)
      : stream(stream),
        timeout(timeout),
        ackValue(ackValue),
        oddParity(oddParity),
        maxTaskQueueSize(maxTaskQueueSize)
  {
  }

  int available()
  {
    return stream.available();
  }

  /**
   * Read a byte from the stream
   * @return a promise that resolves to the read byte
   */
  std::shared_ptr<ReadPromise> read()
  {
    auto promise = std::make_shared<ReadPromise>();

    bool ok = addTask(
        [&, promise]()
        {
          readPromise = promise;
          startTime = millis();
        });

    if (!ok)
      promise->resolve(ReadResult::fail(AsyncSerialError::QUEUE_FULL));

    return promise;
  }

  /**
   * Write a byte to the stream
   * @param data byte to write
   * @return a promise that resolves if the write is successful
   */
  std::shared_ptr<WritePromise> write(char data)
  {
    auto promise = std::make_shared<WritePromise>();

    bool ok = addTask(
        [&, data, promise]()
        {
          lastWriteValue = data;
          put(data);
          writePromise = promise;
          startTime = millis();
        });

    if (!ok)
      promise->resolve(WriteResult::fail(AsyncSerialError::QUEUE_FULL));

    return promise;
  }

  /**
   * Write a string to the stream
   * @param data string to write
   * @return a promise that resolves if the write is successful
   */
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

  /**
   * Update and process the next task
   */
  void update()
  {
    tryProcessNextTask();

    if (available())
      updateWhenAvailable();
    else
      updateWhenNotAvailable();
  }
};