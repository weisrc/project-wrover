#pragma once
#include <Arduino.h>
#include <list>
#include <ArduinoJson.h>

using namespace std;

const static char *EVENT_TYPE_FIELD PROGMEM = "type";
const static char *EVENT_OK_FIELD PROGMEM = "ok";
const static char *EVENT_RESULT_FIELD PROGMEM = "result";
const static char *EVENT_ERROR_FIELD PROGMEM = "error";

const static char *REQUEST_NETWORK_SCAN PROGMEM = "requestNetworkScan";
const static char *REPLY_NETWORK_SCAN PROGMEM = "replyNetworkScan";
const static char *BEGIN_WIFI PROGMEM = "beginWiFi";

const static char *REQUEST_WIFI_STATUS PROGMEM = "requestWiFiStatus";
const static char *REPLY_WIFI_STATUS PROGMEM = "replyWiFiStatus";
const static char *WIFI_STATUS_CHANGE PROGMEM = "wiFiStatusChange";
const static char *DISCONNECT_WIFI PROGMEM = "disconnectWiFi";
const static char *BEGIN_WEB_SERVER PROGMEM = "beginWebServer";

class Context
{
private:
    list<JsonDocument> eventQueue = {};

public:
    bool networkScanPending = false;
    wl_status_t lastWiFiStatus;

    Context()
    {
        lastWiFiStatus = WiFi.status();
    }

    bool hasEvents()
    {
        return eventQueue.size() > 0;
    }

    JsonDocument nextEvent()
    {
        JsonDocument event = eventQueue.front();
        eventQueue.pop_front();
        return event;
    }

    void queueEvent(JsonDocument event)
    {
        eventQueue.push_back(event);
    }

    void sendEvent(JsonDocument event)
    {
        serializeJson(event, Serial);
    }
};