#pragma once
#include <Arduino.h>
#include <ESPAsyncWebServer.h>
#include <ArduinoJson.h>
#include <vector>

AsyncWebServer server(80);
AsyncWebSocket ws("/ws");

bool serverActive = false;
bool scanRequested = false;
wl_status_t lastStatus;

/*
https://github.com/me-no-dev/ESPAsyncWebServer/blob/master/src/AsyncWebSocket.h#L249
Zero seems to be used by the first socket. I will therefore use last one.
*/

#define NO_SOCKET_ID UINT32_MAX
uint32_t cameraSocketId = NO_SOCKET_ID;
uint8_t cameraFps = 10;
unsigned long cameraLastTime = 0;