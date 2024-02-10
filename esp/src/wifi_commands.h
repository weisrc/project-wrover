#pragma once
#include <WiFi.h>

#include "context.h"

void requestNetworkScan()
{
    WiFi.scanNetworks(true, true);
    ctx.networkScanPending = true;
}

void beginWiFi(String command)
{
    const String method = nextWord(command);
    const String ssid = nextWord(command);

    if (method == "open")
        WiFi.begin(ssid);
    else if (method == "psk")
        WiFi.begin(ssid, nextWord(command));
    else if (method == "peap")
        WiFi.begin(ssid, WPA2_AUTH_PEAP,
                   event["id"].as<String>(),
                   event["user"].as<String>(),
                   event["pass"].as<String>());
}

void requestWiFiStatus() {
    JsonDocument reply;
    reply["type"] = REPLY_WIFI_STATUS;
    reply["ok"] = WiFi.status();
    ctx.sendEvent(reply);
}