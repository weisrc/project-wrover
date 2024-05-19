/**
 * @author Wei
 * Web server setup and handlers
 */

#pragma once
#include <memory>

#include "channel.h"
#include "globals.h"
#include "handle_request.h"

/**
 * WebSocket event handler for data
 */
void onWSEventData(AwsFrameInfo *info, AsyncWebSocketClient *client, uint8_t *data, size_t len)
{
  if (info->opcode == WS_TEXT)
  {
    String str((char *)data, len);
    auto chan = std::make_shared<WSChannel>(client);
    messageQueue.add(chan, str);
  }
}

/**
 * WebSocket event handler for connections and data
 */
void onWSEvent(AsyncWebSocket *server, AsyncWebSocketClient *client, AwsEventType type, void *arg,
               uint8_t *data, size_t len)
{
  if (type == WS_EVT_CONNECT)
  {
    WSChannel chan(client);
    sendData(chan, "socketReady", "ok");
  }
  else if (type == WS_EVT_DATA)
    onWSEventData((AwsFrameInfo *)arg, client, data, len);
}

/**
 * Web server setup
 */
void webServerSetup()
{
  webServer.on("/", HTTP_GET,
               [](AsyncWebServerRequest *request)
               { request->send(200, "text/plain", "hello world"); });
  wsEndpoint.onEvent(onWSEvent);
  webServer.addHandler(&wsEndpoint);
}
