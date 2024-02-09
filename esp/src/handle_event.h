#pragma once
#include "context.h"
#include "wifi_commands.h"

void handleEvent(Context* ctx, JsonDocument event)
{

    const String type = event[EVENT_TYPE_FIELD].as<String>();

    if (type == REQUEST_NETWORK_SCAN)
        requestNetworkScan(ctx);
    else if (type == BEGIN_WIFI)
        beginWiFi(ctx, event);
    else if (type == REQUEST_WIFI_STATUS)
        requestWiFiStatus(ctx);
}