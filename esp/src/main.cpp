#include "Arduino.h"
#include "ArduinoJson.h"
// #include <ESPAsyncWebServer.h>

#include "context.h"
#include "check_network_scan.h"
#include "handle_command.h"
#include "check_wifi_status.h"

// AsyncWebServer server(80);

void setup()
{
  Serial.begin(115200);

  // server.on("/", HTTP_GET, [](AsyncWebServerRequest *request)
  //           { request->send(200, "text/plain", "hello world"); });

  // server.begin();
}

void handleCommand(JsonDocument &command)
{

    String type = command["type"];

    if (type == REQUEST_NETWORK_SCAN)
        requestNetworkScan();
    else if (type == BEGIN_WIFI)
        beginWiFi(command);
    else if (type == REQUEST_WIFI_STATUS)
        requestWiFiStatus();
}


void checkNetworkScan()
{
    if (!ctx.networkScanPending)
        return;

    Serial.println("before");

    int16_t value = WiFi.scanComplete();

    Serial.println(value);

    switch (value)
    {
    case WIFI_SCAN_RUNNING:
        break;
    case WIFI_SCAN_FAILED:
    {
        JsonDocument doc;
        doc["type"] = "wifi.scan";
        doc["ok"] = false;
        ctx.send(doc);
        ctx.networkScanPending = false;
        break;
    }
    default:
    {
        Serial.println("Scan complete");
        JsonDocument doc;
        doc["type"] = "wifi.scan";
        doc["ok"] = true;
        JsonArray networks = doc["networks"].to<JsonArray>();
        for (int i = 0; i < value & 8; i++)
        {
            JsonObject network = networks.add<JsonObject>();
            network["ssid"] = WiFi.SSID(i);
            network["rssi"] = WiFi.RSSI(i);
            network["encryption"] = WiFi.encryptionType(i);
            network["channel"] = WiFi.channel(i);
            network["bssid"] = WiFi.BSSIDstr(i);
        }
        ctx.send(doc);
        ctx.networkScanPending = false;
        break;
    }
    }
}


void loop()
{
  if (Serial.available())
  {
    JsonDocument command;
    deserializeJson(command, Serial);
    handleCommand(command);
  }
//   checkWiFiStatus();
  checkNetworkScan();
}
