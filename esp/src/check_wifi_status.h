#pragma once
#include <WiFi.h>
#include "context.h"

void checkWiFiStatus()
{
    auto status = WiFi.status();

    if (status == ctx.lastWiFiStatus)
        return;

    ctx.lastWiFiStatus = status;

    // JsonDocument event;
    // event["type"] = WIFI_STATUS_CHANGE;
    // event["ok"] = status;
    // ctx1.sendEvent(event);

    Serial.println("WiFi status changed");
}