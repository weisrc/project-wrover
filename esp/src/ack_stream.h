/**
 * @author Wei
 * This file is no longer used. It is a stepping stone for AsyncStream.
 */

#pragma once

#include <Arduino.h>

#include "logger.h"

/**
 * Blocking Acknowledgement Stream Wrapper
 */
class AckStream
{
private:
  Stream &stream;
  unsigned long timeout;
  char ackValue;

  /**
   * Get a byte from the stream
   */
  int get()
  {
    unsigned long startTime = millis();

    do
    {
      if (!available())  // block until data is available
        continue;

      char data = stream.read();
      LOG_DEBUG("RX: " + String(data) + " (" + String((int)data) + ")");
      return data;
    } while (millis() - startTime < timeout);  // Timeout

    LOG_WARN("RX: Timeout");
    return -1;
  }

  /**
   * Put a byte to the stream
   */
  void put(char data)
  {
    stream.write(data);
    LOG_DEBUG("TX: " + String(data) + " (" + String((int)data) + ")");
  }

public:
  AckStream(Stream &stream, unsigned long timeout = 50, char ackValue = 0)
      : stream(stream), timeout(timeout), ackValue(ackValue)
  {
  }

  int available()
  {
    return stream.available();
  }

  /**
   * Read a byte from the stream and send an acknowledgement
   */
  int read()
  {
    int data = get();
    if (data != -1)
    {
      LOG_DEBUG("Sending ack for " + String(data) + " (" + String((int)data) + ")");
      put(ackValue);
    }
    return data;
  }

  /**
   * Write a byte to the stream and wait for an acknowledgement
   */
  void write(char data)
  {
    do
      put(data);
    while (get() == -1);
  }
};