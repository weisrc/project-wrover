#pragma once
#include <WiFi.h>

#include "context.h"

void requestNetworkScan(Context* ctx)
{
    WiFi.scanDelete();
    WiFi.scanNetworks(true, true);
    ctx->networkScanPending = true;
}

void beginWiFi(Context* ctx, JsonDocument event)
{
    const String method = event["method"].as<String>();
    const String ssid = event["ssid"].as<String>();

    if (method == "open")
        WiFi.begin(ssid);
    else if (method == "psk")
        WiFi.begin(ssid, event["psk"].as<String>());
    else if (method == "peap")
        WiFi.begin(ssid, WPA2_AUTH_PEAP,
                   event["id"].as<String>(),
                   event["user"].as<String>(),
                   event["pass"].as<String>());
}

void requestWiFiStatus(Context* ctx) {
    JsonDocument reply;
    reply[EVENT_TYPE_FIELD] = REPLY_WIFI_STATUS;
    reply[EVENT_RESULT_FIELD] = WiFi.status();
    ctx->sendEvent(reply);
}