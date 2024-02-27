#pragma once
#include <Arduino.h>
#include <ArduinoJson.h>

#include "channel.h"
#include "globals.h"
#include "data_utils.h"
#include "wifi_connection.h"

void begin(Channel &chan)
{
  webServer.begin();
  sendData(chan, "begin", "ok");
  webServerActive = true;
}

void handleRequest(Channel &chan, JsonDocument &request)
{
  String type = request["type"];
  JsonDocument echo;
  echo["type"] = "echo";
  echo["data"] = type;
  chan.send(echo);
  if (type == "scan")
  {
    scanRequested = true;
    WiFi.scanNetworks(true);
  }
  else if (type == "connect")
    connect(chan, request);
  else if (type == "disconnect")
    disconnect(chan);
  else if (type == "ip")
    sendData(chan, "ip", WiFi.localIP().toString());
  else if (type == "rssi")
    sendData(chan, "rssi", String(WiFi.RSSI()));
  else if (type == "status")
    sendData(chan, "status", String(WiFi.status()));
  else if (type == "ssid")
    sendData(chan, "ssid", WiFi.SSID());
  else if (type == "begin")
    begin(chan);
  else if (type == "beginCamera")
    cameraSocketId = chan.socketId();
  else if (type == "stopCamera")
    cameraSocketId = NO_SOCKET_ID;
  else if (type == "setCameraFPS") {
    cameraFps = request["fps"];
  }
}
