#include <Arduino.h>
#include <ESPAsyncWebServer.h>
#include <ArduinoJson.h>

#include "context.h"
#include "check_network_scan.h"
#include "handle_event.h"
#include "check_wifi_status.h"

using namespace std;

AsyncWebServer server(80);

Context ctx;

void setup()
{
  Serial.begin(115200);

  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request)
            { request->send(200, "text/plain", "hello world"); });

  // server.begin();
}

void loop()
{
  if (Serial.available())
  {
    JsonDocument event;
    DeserializationError error = deserializeJson(event, Serial);
    handleEvent(&ctx, event);
  }
  checkNetworkScan(&ctx);
  checkWiFiStatus(&ctx);
}
