#pragma once
#include <Arduino.h>
#include <ESPAsyncWebServer.h>
#include <ArduinoJson.h>
#include <SoftwareSerial.h>

#define NO_SOCKET_ID UINT32_MAX
#define HALL0 GPIO_NUM_32
#define HALL1 GPIO_NUM_13
#define AVR_RX GPIO_NUM_33
#define AVR_TX GPIO_NUM_14

#define STORAGE_SIZE 2048
#define HALL_SIZE 2024

uint8_t cameraFps = 5;
bool webServerActive = false;
bool scanRequested = false;
wl_status_t lastStatus;

AsyncWebServer webServer(80);
AsyncWebSocket wsEndpoint("/ws");
AsyncWebSocket camEndpoint("/cam");
SoftwareSerial avrSerial;
