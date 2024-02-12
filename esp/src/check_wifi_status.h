#pragma once
#include <WiFi.h>
#include "context.h"

void checkWiFiStatus()
{
    auto status = WiFi.status();

    if (status == ctx.lastWiFiStatus)
        return;

    ctx.lastWiFiStatus = status;

    JsonDocument event;
    event["type"] = WIFI_STATUS_CHANGE;
    event["ok"] = true;
    // event["status"] = status;
    ctx.send(event);

    Serial.println("WiFi status changed");
}