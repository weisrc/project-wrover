#pragma once
#include <Arduino.h>
#include <ArduinoJson.h>

#include "globals.h"
#include "data_utils.h"

void checkScanComplete()
{

  if (!scanRequested || WiFi.scanComplete() < 0)
    return;

  scanRequested = false;
  JsonDocument reply;
  reply["type"] = "scan";
  JsonArray networks = reply["networks"].to<JsonArray>();
  for (int i = 0; i < WiFi.scanComplete(); i++)
  {
    JsonArray network = networks.add<JsonArray>();
    network.add(WiFi.SSID(i));
    network.add(WiFi.RSSI(i));
    network.add(WiFi.encryptionType(i));
    network.add(WiFi.channel(i));
    network.add(WiFi.BSSIDstr(i));
  }
  broadcast(reply);
}

void checkStatusChange()
{
  wl_status_t status = WiFi.status();
  if (status != lastStatus)
  {
    lastStatus = status;
    broadcastData("status", String(status));
  }
}