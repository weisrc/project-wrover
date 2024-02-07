#include <Arduino.h>
#include <WiFi.h>

void setup()
{
  Serial.begin(115200);
  while (Serial.available() == 0)
  {
    delay(100);
  }
  Serial.read();

  int n = WiFi.scanNetworks();
  for (int i = 0; i < n; ++i)
  {
    Serial.print(WiFi.RSSI(i));
    Serial.print("\t");
    Serial.print(WiFi.encryptionType(i));
    Serial.print("\t");
    Serial.println(WiFi.SSID(i));
  }


}

void loop()
{
}
