#pragma once
#include <Arduino.h>
#include <ESPAsyncWebServer.h>
#include <ArduinoJson.h>

class Channel
{
public:
  virtual void send(JsonDocument &data) = 0;
  virtual uint32_t socketId() = 0;
};

class NullChannel : public Channel
{
  void send(JsonDocument &data)
  {
  }
  uint32_t socketId()
  {
    return NO_SOCKET_ID;
  }
};

class SerialChannel : public Channel
{
public:
  void send(JsonDocument &data)
  {
    serializeJson(data, Serial);
    Serial.println();
  }
  uint32_t socketId()
  {
    return NO_SOCKET_ID;
  }
};

class WSChannel : public Channel
{
private:
  AsyncWebSocketClient *client;

public:
  WSChannel(AsyncWebSocketClient *client)
  {
    this->client = client;
  }
  void send(JsonDocument &data)
  {
    String output;
    serializeJson(data, output);
    this->client->text(output);
  }
  uint32_t socketId()
  {
    return client->id();
  }
};