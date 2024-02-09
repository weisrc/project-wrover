#pragma once
#include "WiFi.h"
#include "context.h"

void checkWiFiStatus(Context* ctx)
{
    auto status = WiFi.status();

    if (status == ctx->lastWiFiStatus)
        return;

    ctx->lastWiFiStatus = status;

    JsonDocument event;
    event[EVENT_TYPE_FIELD] = WIFI_STATUS_CHANGE;
    event[EVENT_RESULT_FIELD] = status;
    ctx->sendEvent(event);
}