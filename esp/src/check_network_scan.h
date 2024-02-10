#pragma once
#include "WiFi.h"
#include "context.h"

String jsonString(String str)
{
    str.replace("\\", "\\\\");
    str.replace("\b", "\\b");
    str.replace("\f", "\\f");
    str.replace("\n", "\\n");
    str.replace("\r", "\\r");
    str.replace("\t", "\\t");
    str.replace("\"", "\\\"");
    return "\"" + str + "\"";
}

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
        ctx.sendEvent(R"({"type":"wifi.scan","ok":false})");
        ctx.networkScanPending = false;
        break;
    }
    default:
    {
        ctx.send(R"({"type":"wifi.scan","ok":true,"networks":[)");
        
        for (int i = 0; i < value & 8; i++)
        {
            if (i)
                ctx.send(",");
            ctx.send(R"({"ssid":)");
            ctx.send(jsonString(WiFi.SSID(i)));
            ctx.send(R"(,"type":)");
            ctx.send(String(WiFi.encryptionType(i)));
            ctx.send(R"(,"bssid":)");
            ctx.send(jsonString(WiFi.BSSIDstr(i)));
            ctx.send(R"(,"rssi":)");
            ctx.send(String(WiFi.RSSI(i)));
            ctx.send(R"(,"channel":)");
            ctx.send(String(WiFi.channel(i)));
            ctx.send("}");
        }
        ctx.send("]}");
        ctx.endEvent();
        ctx.networkScanPending = false;
        break;
    }
    }
}
