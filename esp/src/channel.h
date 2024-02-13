#pragma once
#include <Arduino.h>
#include <ESPAsyncWebServer.h>
#include <ArduinoJson.h>

class Channel
{
public:
  virtual void send(JsonDocument &data) = 0;
};

class SerialChannel : public Channel
{
public:
  void send(JsonDocument &data)
  {
    serializeJson(data, Serial);
    Serial.println();
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
};