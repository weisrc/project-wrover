#include <Arduino.h>
#include <ESPAsyncWebServer.h>
#include <ArduinoJson.h>
#include <StreamUtils.h>

#include "camera_setup.h"
#include "camera_stream.h"
#include "avr_serial.h"
#include "globals.h"
#include "wifi_checks.h"
#include "web_server.h"
#include "handle_request.h"
#include "locomotion.h"
#include "logger.h"

void update()
{
  avrAckStream.update();
  checkScanComplete();
  checkStatusChange();
  locomotionUpdate();
  motorUpdate();
  sonarUpdate();
  cameraCapture();
  wsEndpoint.cleanupClients();

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
}

void sleep(int seconds)
{
  unsigned long startTime = millis();
  while (millis() - startTime < seconds)
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
  sleep(1);

  LOG_INFO("Setting up camera...");

  cameraSetup();
  webServerSetup();
  hallSensorSetup();

  WiFi.mode(WIFI_STA);
  WiFi.disconnect();

  lastStatus = WiFi.status();
  autoConnect();
}

void loop()
{
  update();
}
