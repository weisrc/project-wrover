#pragma once
#include <Arduino.h>
#include "ArduinoJson.h"
#include <WiFi.h>

#define REQUEST_NETWORK_SCAN "requestNetworkScan"
#define REPLY_NETWORK_SCAN "replyNetworkScan"
#define BEGIN_WIFI "beginWiFi"

#define REQUEST_WIFI_STATUS "requestWiFiStatus"
#define REPLY_WIFI_STATUS "replyWiFiStatus"
#define WIFI_STATUS_CHANGE "wiFiStatusChange"
#define DISCONNECT_WIFI "disconnectWiFi"
#define BEGIN_WEB_SERVER "beginWebServer"

class Context
{

public:
    bool networkScanPending = false;
    // wl_status_t lastWiFiStatus;

    Context()
    {
        // lastWiFiStatus = WiFi.status();
    }

    void send(JsonDocument &doc)
    {
        if (Serial.availableForWrite())
        {
            serializeJson(doc, Serial);
        }
    }
};

Context ctx;