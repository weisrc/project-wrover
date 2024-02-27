#pragma once
#include <Arduino.h>
#include <ESPAsyncWebServer.h>
#include <ArduinoJson.h>
#include <SoftwareSerial.h>

/*
https://github.com/me-no-dev/ESPAsyncWebServer/blob/master/src/AsyncWebSocket.h#L249
Zero seems to be used by the first socket. I will therefore use last one.
*/
#define NO_SOCKET_ID UINT32_MAX
#define HALL1 GPIO_NUM_32
#define HALL2 GPIO_NUM_13
#define AVR_RX GPIO_NUM_33
#define AVR_TX GPIO_NUM_14

#define STORAGE_SIZE 2048

uint32_t cameraSocketId = NO_SOCKET_ID;
uint8_t cameraFps = 10;
unsigned long cameraLastTime = 0;

bool webServerActive = false;
bool scanRequested = false;
wl_status_t lastStatus;

AsyncWebServer webServer(80);
AsyncWebSocket wsEndpoint("/ws");
SoftwareSerial avrSerial;
