#pragma once

#include <Arduino.h>

#include "logger.h"

class AckStream
{
private:
  Stream &stream;
  unsigned long timeout;
  char ackValue;

  int get()
  {
    unsigned long startTime = millis();

    do
    {
      if (!available())
        continue;

      char data = stream.read();
      LOG_DEBUG("RX: " + String(data) + " (" + String((int)data) + ")");
      return data;
    } while (millis() - startTime < timeout);

    LOG_WARN("RX: Timeout");
    return -1;
  }

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

  void write(char data)
  {
    do
      put(data);
    while (get() == -1);
  }
};