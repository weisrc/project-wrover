#pragma once
#include <WiFi.h>
#include "serial_utils.h"

int printWiFiTable()
{
    WiFi.mode(WIFI_STA);
    int n = WiFi.scanNetworks();
    Serial.println("[TABLE START]");
    Serial.println("ID\tRSSI\tTYPE\tSSID");
    for (int i = 0; i < n; i++)
    {
        Serial.print(i + 1);
        Serial.print('\t');
        Serial.print(WiFi.RSSI(i));
        Serial.print('\t');
        Serial.print(WiFi.encryptionType(i));
        Serial.print('\t');
        Serial.println(WiFi.SSID(i));
    }
    Serial.println("[TABLE END]");
    return n;
}

void beginWPA2Entreprise(String ssid)
{
    int authMethod = promptInt("[CHOICE] TLS=1, PEAP=2, TTLS=3", 1, 3) - 1;
    String identity = promptString("Identity");
    String username = promptString("Username (Leave blank to use identity)");
    if (!username.length())
        username = identity;
    String password = promptString("Password", true);
    WiFi.begin(ssid, (wpa2_auth_method_t)authMethod, identity, username, password);
}

bool beginWiFi(int n)
{
    int id = 0;

    while (!id)
    {
        id = promptInt("ID");
        if (id > n)
            id = 0;
    }

    id -= 1;

    String ssid = WiFi.SSID(id);
    wifi_auth_mode_t authMode = WiFi.encryptionType(id);

    switch (authMode)
    {
    case WIFI_AUTH_OPEN:
        WiFi.begin(ssid);
        break;
    case WIFI_AUTH_WPA2_PSK:
    {
        String psk = promptString("PSK", true);
        WiFi.begin(ssid, psk);
        break;
    }
    case WIFI_AUTH_WPA2_ENTERPRISE:
        beginWPA2Entreprise(ssid);
        break;
    default:
        return false;
    }

    Serial.println("Starting connection...");

    while (WiFi.status() != WL_CONNECTED)
    {
        delay(1000);
        if (WiFi.status() == WL_CONNECT_FAILED || WiFi.status() == WL_DISCONNECTED)
        {
            Serial.println("Connection failed! Please retry...");
            return false;
        }
        else
        {
            Serial.println("Connecting...");
        }
    }

    Serial.println("Connected!");
    Serial.println(WiFi.localIP());


    return true;
}

void connectWiFi()
{

    Serial.println("Press enter to start...");
    waitForChar('\n');
    Serial.println("Scanning networks...");
    int n = printWiFiTable();
    while (!beginWiFi(n))
        continue;

    
}