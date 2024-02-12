#pragma once
#include "context.h"
#include "wifi_commands.h"




void handleCommand(JsonDocument &command)
{

    auto type = command["type"];

    if (type == REQUEST_NETWORK_SCAN)
        requestNetworkScan();
    else if (type == BEGIN_WIFI)
        beginWiFi(command);
    else if (type == REQUEST_WIFI_STATUS)
        requestWiFiStatus();
}