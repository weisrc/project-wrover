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
  setCpuFrequencyMhz(240);
  
  Serial.begin(115200);
  EEPROM.begin(STORAGE_SIZE);
  
  avrSerialSetup();
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
