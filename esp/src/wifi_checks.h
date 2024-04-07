#pragma once
#include <Arduino.h>
#include <ArduinoJson.h>

#include "globals.h"
#include "data_utils.h"
#include "avr_serial.h"
#include "begin_webserver.h"
#include "wifi_connection.h"

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

    if (status == WL_CONNECTED)
    {
      avrClear();
      avrPrint("WiFi Connected:\n");
      avrPrint(WiFi.localIP().toString());
      NullChannel chan;
      beginWebServer(chan);
    }
    else if (status == WL_CONNECT_FAILED)
    {
      NullChannel chan;
      disconnect(chan);
      avrClear();
      avrPrint("WiFi Failed\nSetup required");
      LOG_WARN("WiFi connection failed");
    }
    else if (status == WL_NO_SSID_AVAIL)
    {
      NullChannel chan;
      disconnect(chan);
      avrClear();
      avrPrint("WiFi No SSID\nSetup required");
      LOG_WARN("No SSID available");
    }
  }
}