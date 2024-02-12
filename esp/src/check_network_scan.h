#pragma once
#include "WiFi.h"
#include "context.h"
#include "ArduinoJson.h"

void checkNetworkScan()
{
    if (!ctx.networkScanPending)
        return;

    auto value = WiFi.scanComplete();

    switch (value)
    {
    case WIFI_SCAN_RUNNING:
        break;
    case WIFI_SCAN_FAILED:
    {
        JsonDocument doc;
        doc["type"] = "wifi.scan";
        doc["ok"] = false;
        ctx.send(doc);
        ctx.networkScanPending = false;
        break;
    }
    default:
    {
        Serial.println("Scan complete");
        JsonDocument doc;
        doc["type"] = "wifi.scan";
        doc["ok"] = true;
        JsonArray networks = doc["networks"].to<JsonArray>();
        for (int i = 0; i < value & 8; i++)
        {
            JsonObject network = networks.createNestedObject();
            network["ssid"] = WiFi.SSID(i);
            network["rssi"] = WiFi.RSSI(i);
            network["encryption"] = WiFi.encryptionType(i);
            network["channel"] = WiFi.channel(i);
            network["bssid"] = WiFi.BSSIDstr(i);
        }
        ctx.send(doc);
        ctx.networkScanPending = false;
        break;
    }
    }
}
