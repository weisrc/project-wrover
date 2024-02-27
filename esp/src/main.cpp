#include <Arduino.h>
#include <ESPAsyncWebServer.h>
#include <ArduinoJson.h>

#include "camera_setup.h"
#include "camera_stream.h"
#include "avr_serial.h"
#include "globals.h"
#include "wifi_checks.h"
#include "web_server.h"
#include "handle_request.h"
#include "EEPROM.h"
#include "StreamUtils.h"

void setup()
{
  Serial.begin(115200);
  EEPROM.begin(STORAGE_SIZE);
  avrSerialSetup();
  cameraSetup();
  webServerSetup();

  lastStatus = WiFi.status();
  avrClear();
  avrPrint("WRover ESP\nWaiting Setup...");

  JsonDocument connectJson;
  EepromStream eepromStream(0, STORAGE_SIZE);
  DeserializationError error = deserializeJson(connectJson, eepromStream);


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

  if (avrSerial.available()) {
    Serial.write(avrSerial.read());
  }


  checkScanComplete();
  checkStatusChange();
  cameraStream();
}
