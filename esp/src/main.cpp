#include <Arduino.h>
#include <ESPAsyncWebServer.h>
#include <ArduinoJson.h>

#include "camera_setup.h"
#include "channel.h"
#include "globals.h"
#include "wifi_checks.h"
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
  if (type == WS_EVT_CONNECT) {
    WSChannel chan(client);
    sendData(chan, "socketReady", "ok");
  }
  else if (type == WS_EVT_DATA)
    onWSEventData((AwsFrameInfo *)arg, client, data, len);
}

void setup()
{
  Serial.begin(115200);

  lastStatus = WiFi.status();

  cameraSetup();

  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request)
            { request->send(200, "text/plain", "hello world"); });
  ws.onEvent(onWSEvent);
  server.addHandler(&ws);
}

void loop()
{
  if (Serial.available())
  {
    SerialChannel chan;
    JsonDocument request;
    DeserializationError error = deserializeJson(request, Serial);
    if (!error)
      handleRequest(chan, request);
  }

  checkScanComplete();
  checkStatusChange();
}
