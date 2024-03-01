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

void setup()
{
  Serial.begin(115200);
  EEPROM.begin(STORAGE_SIZE);
  
  avrSerialSetup();
  cameraSetup();
  webServerSetup();
  hallSensorSetup();

  lastStatus = WiFi.status();

  sleep(1);

  avrClear();
  JsonDocument connectJson;
  EepromStream eepromStream(0, STORAGE_SIZE);
  DeserializationError error = deserializeJson(connectJson, eepromStream);
  if (!error && connectJson["type"] == "connect")
  {
    NullChannel chan;
    connect(chan, connectJson);
    avrPrint("WRover ESP\nConnecting...");
  }
  else
  {
    avrPrint("WRover ESP\nWaiting Setup...");
  }
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

  if (avrSerial.available())
  {
    Serial.write(avrSerial.read());
  }

  checkScanComplete();
  checkStatusChange();
  cameraStream();
  locomotionBroadcast();
}
