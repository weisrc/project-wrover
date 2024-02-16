#pragma once
#include <ArduinoJson.h>
#include "globals.h"
#include "channel.h"

void createData(JsonDocument &doc, String type, String data)
{
  doc["type"] = type;
  doc["data"] = data;
}

void broadcast(JsonDocument &doc)
{
  serializeJson(doc, Serial);
  Serial.println();

  if (serverActive)
  {
    String out;
    serializeJson(doc, out);
    ws.textAll(out);
  }
}

void broadcastData(String type, String data)
{
  JsonDocument doc;
  createData(doc, type, data);
  broadcast(doc);
}

void sendData(Channel &chan, String type, String data)
{
  JsonDocument doc;
  createData(doc, type, data);
  chan.send(doc);
}