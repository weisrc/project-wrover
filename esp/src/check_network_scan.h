#pragma once
#include "WiFi.h"
#include "context.h"

void checkNetworkScan(Context* ctx)
{
    if (!ctx->networkScanPending)
        return;

    auto value = WiFi.scanComplete();

    switch (value)
    {
    case WIFI_SCAN_RUNNING:
        break;
    case WIFI_SCAN_FAILED:
    {
        JsonDocument reply;
        reply[EVENT_TYPE_FIELD] = REPLY_NETWORK_SCAN;
        reply[EVENT_OK_FIELD] = false;
        ctx->sendEvent(reply);
        ctx->networkScanPending = false;
    }
    default:
    {
        JsonDocument reply;
        reply[EVENT_TYPE_FIELD] = REPLY_NETWORK_SCAN;
        reply[EVENT_OK_FIELD] = true;
        JsonDocument networksDoc;
        JsonArray networks = networksDoc.to<JsonArray>();

        for (int i = 0; i < value; i++)
        {
            JsonDocument network;
            network["ssid"] = WiFi.SSID(i);
            network["type"] = WiFi.encryptionType(i);
            network["bssid"] = WiFi.BSSID(i);
            network["rssi"] = WiFi.RSSI(i);
            networks.add(network);
        }
        reply["networks"] = networks;
        ctx->sendEvent(reply);
        ctx->networkScanPending = false;
    }
    }
}
