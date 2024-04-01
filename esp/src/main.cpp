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

  Serial.println("Starting AVR serial...");

  avrSerialSetup();

  while (1)
  {
    int sonar0 = avrSonar(MODE_SONAR0);
    int sonar1 = avrSonar(MODE_SONAR1);
    int sonar2 = avrSonar(MODE_SONAR2);
    Serial.println("Sonar0: " + String(sonar0) + " Sonar1: " + String(sonar1) + " Sonar2: " + String(sonar2));
    sleep(10);
  }

  Serial.println("Clearing AVR text...");

  avrClear();

  Serial.println("Starting WRover ESP...");

  avrPrint("WRover ESP\nStarting...");
  sleep(1);

  Serial.println("Setting up camera...");

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
    else
      Serial.println("failed to parse");
  }

  if (avrSerial.available())
  {
    Serial.write(avrSerial.read());
  }

  checkScanComplete();
  checkStatusChange();
  locomotionUpdate();
  motorUpdate();
  sonarUpdate();
  cameraCapture();
  wsEndpoint.cleanupClients();
}
