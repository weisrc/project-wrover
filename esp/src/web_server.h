#pragma once
#include "globals.h"
#include "channel.h"
#include "handle_request.h"

void onWSEventData(AwsFrameInfo *info, AsyncWebSocketClient *client, uint8_t *data, size_t len)
{
    if (info->opcode == WS_TEXT)
    {
        JsonDocument request;
        WSChannel chan(client);
        DeserializationError error = deserializeJson(request, data, len);
        if (!error)
            handleRequest(chan, request);
    }
}

void onWSEvent(AsyncWebSocket *server, AsyncWebSocketClient *client, AwsEventType type, void *arg, uint8_t *data, size_t len)
{
    if (type == WS_EVT_CONNECT)
    {
        WSChannel chan(client);
        sendData(chan, "socketReady", "ok");
    }
    else if (type == WS_EVT_DATA)
        onWSEventData((AwsFrameInfo *)arg, client, data, len);
}

void webServerSetup()
{
    webServer.on("/", HTTP_GET, [](AsyncWebServerRequest *request)
              { request->send(200, "text/plain", "hello world"); });
    wsEndpoint.onEvent(onWSEvent);
    webServer.addHandler(&wsEndpoint);
}