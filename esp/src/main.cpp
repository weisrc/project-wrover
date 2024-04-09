#include <Arduino.h>
#include <ArduinoJson.h>
#include <ESPAsyncWebServer.h>
#include <StreamUtils.h>

#include "avr_serial.h"
#include "camera_setup.h"
#include "camera_stream.h"
#include "globals.h"
#include "handle_request.h"
#include "locomotion.h"
#include "logger.h"
#include "web_server.h"
#include "wifi_checks.h"

void update()
{
  avrSerial.update();
  checkScanComplete();
  checkStatusChange();
  locomotionUpdate();
  motorUpdate();
  sonarUpdate();
  cameraCapture();
  wsEndpoint.cleanupClients();

  if (!messageQueue.empty())
  {
    auto message = messageQueue.get();
    JsonDocument request;
    auto error = deserializeJson(request, message.data);
    if (!error)
      handleRequest(*message.channel, request);
  }

  if (Serial.available())
  {
    SerialChannel chan;
    JsonDocument request;
    DeserializationError error = deserializeJson(request, Serial);
    if (!error)
      handleRequest(chan, request);
    else
      LOG_WARN("failed to parse");
  }

  static unsigned long lastUpdate = 0;
  if (millis() - lastUpdate > 1000)
  {
    lastUpdate = millis();
    LOG_INFO("Free heap: " + String(ESP.getFreeHeap()));
  }
}

void sleepUpdate(unsigned long timeout)
{
  unsigned long startTime = millis();
  while (millis() - startTime < timeout)
    update();
}

void setup()
{
  setCpuFrequencyMhz(240);

  Serial.begin(115200);
  EEPROM.begin(STORAGE_SIZE);

  LOG_INFO("Starting AVR serial...");

  avrSerialSetup();

  LOG_INFO("Starting WRover ESP...");

  avrClear();
  avrPrint("WRover ESP\nStarting...");

  LOG_INFO("Setting up camera...");

  cameraSetup();
  webServerSetup();
  hallSensorSetup();

  WiFi.mode(WIFI_STA);
  WiFi.disconnect();

  lastStatus = WiFi.status();

  sleepUpdate(1000);

  autoConnect();
}

void loop()
{
  update();
}
