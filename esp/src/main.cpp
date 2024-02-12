#include "Arduino.h"
#include "ArduinoJson.h"
// #include <ESPAsyncWebServer.h>

#include "context.h"
#include "check_network_scan.h"
#include "handle_command.h"
#include "check_wifi_status.h"

// AsyncWebServer server(80);

char serialCommand[64];
size_t serialIndex = 0;
char socketCommand[64];
size_t socketIndex = 0;

void setup()
{
  Serial.begin(115200);

  // server.on("/", HTTP_GET, [](AsyncWebServerRequest *request)
  //           { request->send(200, "text/plain", "hello world"); });

  // server.begin();
}

void loop()
{
  if (Serial.available())
  {
    JsonDocument command;
    deserializeJson(command, Serial);
    handleCommand(command);
  }
  checkNetworkScan();
  checkWiFiStatus();

}
