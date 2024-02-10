#include <Arduino.h>
// #include <ESPAsyncWebServer.h>
#include <ArduinoJson.h>

#include "context.h"
#include "check_network_scan.h"
#include "handle_command.h"
#include "check_wifi_status.h"
#include "connect_wifi.h"

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
    char c = Serial.read();
    if (c == '\n')
    {
      serialCommand[serialIndex] = 0;
      handleCommand(serialCommand);
      serialIndex = 0;
    }
    else
    {
      serialCommand[serialIndex++] = c;
    }
  }
  checkNetworkScan();
  checkWiFiStatus();

}
