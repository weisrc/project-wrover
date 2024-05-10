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
#include "vec2.h"

#define HALL0 GPIO_NUM_32
#define HALL1 GPIO_NUM_13
#define AVR_RX GPIO_NUM_14
#define AVR_TX GPIO_NUM_33

#define STORAGE_SIZE 2048
#define HALL_SIZE 2024
#define AVR_SERIAL_TIMEOUT 50

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
MessageQueue messageQueue;

DualOdometer odometer(0.16, 0.043);
bool navigationEnabled = false;
Vec2 targetPosition;

int8_t motor0Speed = 0;  // target speed
int8_t motor1Speed = 0;

uint16_t sonar0Distance = 0;
uint16_t sonar1Distance = 0;
uint16_t sonar2Distance = 0;

bool hall0Changed = false;
bool hall1Changed = false;
bool motor0Reverse = false;
bool motor1Reverse = false;
String hall;  // string with LRlr for hall data
