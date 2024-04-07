#pragma once
#include <Arduino.h>
#include <ESPAsyncWebServer.h>
#include <ArduinoJson.h>

#define NO_SOCKET_ID UINT32_MAX

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
  uint32_t id;
  AsyncWebSocket *server;

public:
  WSChannel(AsyncWebSocketClient *client) : server(client->server()), id(client->id()) {}

  void send(JsonDocument &data)
  {
    String output;
    serializeJson(data, output);
    server->text(id, output);
  }
  uint32_t socketId()
  {
    return id;
  }
};