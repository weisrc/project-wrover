/**
 * @author Wei
 * Data utilities for creating and sending JSON data
 */

#pragma once
#include <ArduinoJson.h>

#include "channel.h"
#include "globals.h"

/**
 * Create a JSON object with type and data
 */
template <typename T>
void createData(JsonDocument &doc, String type, T data)
{
  doc["type"] = type;
  doc["data"] = data;
}

/**
 * Broadcast a JSON object
 */
void broadcast(JsonDocument &doc)
{
  serializeJson(doc, Serial);
  Serial.println();

  if (webServerActive)  // send to WebSocket clients only if web server is active
  {
    String out;
    serializeJson(doc, out);
    wsEndpoint.textAll(out);
  }
}

/**
 * Broadcast data with type and data
 */
template <typename T>
void broadcastData(String type, T data)
{
  JsonDocument doc;
  createData(doc, type, data);
  broadcast(doc);
}

/**
 * Send data to a channel
 */
template <typename T>
void sendData(Channel &chan, String type, T data)
{
  JsonDocument doc;
  createData(doc, type, data);
  chan.send(doc);
}