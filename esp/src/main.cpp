#include <Arduino.h>
#include "connect_wifi.h"

void setup()
{
  Serial.begin(115200);
  connectWiFi();
}

void loop()
{
}
