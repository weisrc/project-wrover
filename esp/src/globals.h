/**
 * @author Wei
 * Global variables/singletons
 * Created to prevent circular dependencies
 */

#pragma once
#include <Arduino.h>
#include <ArduinoJson.h>
#include <ESPAsyncWebServer.h>
#include <SoftwareSerial.h>

#include "async_serial.h"
#include "dual_odometer.h"
#include "message_queue.h"

#define HALL0 GPIO_NUM_32
#define HALL1 GPIO_NUM_13
#define AVR_RX GPIO_NUM_14
#define AVR_TX GPIO_NUM_33

#define STORAGE_SIZE 2048
#define HALL_SIZE 2024
#define AVR_SERIAL_TIMEOUT 100

uint32_t camSocketId = NO_SOCKET_ID;
int cameraFps = 24;
bool cameraOk = false;
bool webServerActive = false;
bool scanRequested = false;
wl_status_t lastStatus;

AsyncWebServer webServer(80);
AsyncWebSocket wsEndpoint("/ws");
SoftwareSerial avrSerialBase;
AsyncSerial avrSerial(avrSerialBase, AVR_SERIAL_TIMEOUT, 0, true);
DualOdometer odometer(15, 0.043);
MessageQueue messageQueue;