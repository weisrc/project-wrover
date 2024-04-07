#pragma once
#include <ArduinoJson.h>
#include "globals.h"
#include "channel.h"

template <typename T>
void createData(JsonDocument &doc, String type, T data)
{
  doc["type"] = type;
  doc["data"] = data;
}

void broadcast(JsonDocument &doc)
{
  serializeJson(doc, Serial);
  Serial.println();

  if (webServerActive)
  {
    String out;
    serializeJson(doc, out);
    wsEndpoint.textAll(out);
  }
}

template <typename T>
void broadcastData(String type, T data)
{
  JsonDocument doc;
  createData(doc, type, data);
  broadcast(doc);
}

template <typename T>
void sendData(Channel &chan, String type, T data)
{
  JsonDocument doc;
  createData(doc, type, data);
  chan.send(doc);
}