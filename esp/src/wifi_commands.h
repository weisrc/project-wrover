#pragma once
#include <WiFi.h>

#include "context.h"

void requestNetworkScan()
{
    WiFi.scanNetworks(true, true);
    ctx.networkScanPending = true;
}

void beginWiFi(JsonDocument &command)
{
    const String method = command["method"];
    const String ssid = command["ssid"];

    if (method == "open")
        WiFi.begin(ssid);
    else if (method == "psk")
        WiFi.begin(ssid, command["pass"].as<String>());
    else if (method == "peap")
        WiFi.begin(ssid, WPA2_AUTH_PEAP,
                   command["id"].as<String>(),
                   command["user"].as<String>(),
                   command["pass"].as<String>());
}

void requestWiFiStatus()
{
    JsonDocument reply;
    reply["type"] = REPLY_WIFI_STATUS;
    reply["ok"] = WiFi.status();
    ctx.send(reply);
}