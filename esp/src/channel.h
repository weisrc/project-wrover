/**
 * @author Wei
 * Channel class is equivalent to Java's interface
 * NullChannel is a dummy channel that does nothing
 * SerialChannel is a channel that sends data to Serial
 * WSChannel is a channel that sends data to a WebSocket client
 */

#pragma once
#include <Arduino.h>
#include <ArduinoJson.h>
#include <ESPAsyncWebServer.h>

#define NO_SOCKET_ID UINT32_MAX

class Channel
{
public:
  virtual void send(JsonDocument &data) = 0;
  virtual uint32_t socketId() = 0;
};

/**
 * Sink hole for data
 * This is done for compatibility with functions/methods
 * using Channel without causing side effects or implementing new code
 */
class NullChannel : public Channel
{
  void send(JsonDocument &data) {}
  uint32_t socketId()
  {
    return NO_SOCKET_ID;
  }
};

/**
 * Doesn't have a socket ID because it's not a WebSocket client
 */
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

/**
 * WebSocket channel
 */
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