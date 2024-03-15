#pragma once
#include <Arduino.h>

#include "globals.h"
#include "data_utils.h"
#include "avr_serial.h"

bool hall0Changed = false;
bool hall1Changed = false;
String hall;

void setMotor(Channel &chan, JsonDocument &request) {
  avrSend(MODE_MOTOR0, request["m0"].as<int8_t>());
  avrSend(MODE_MOTOR1, request["m1"].as<int8_t>());
}

void IRAM_ATTR hall0ISR()
{
    hall0Changed = true;
}

void IRAM_ATTR hall1ISR()
{
    hall1Changed = true;
}

void hallSensorSetup()
{
    pinMode(HALL0, INPUT);
    pinMode(HALL1, INPUT);
    attachInterrupt(HALL0, hall0ISR, CHANGE);
    attachInterrupt(HALL1, hall1ISR, CHANGE);
    hall.reserve(HALL_SIZE);
}

void broadcastLocomotion() {
    JsonDocument data;
    data["type"] = "locomotion";
    data["hall"] = hall;
    JsonArray sonars = data["sonar"].to<JsonArray>();
    sonars.add(avrSonar(MODE_SONAR0));
    sonars.add(avrSonar(MODE_SONAR1));
    sonars.add(avrSonar(MODE_SONAR2));
    broadcast(data);
    hall.clear();
}

void locomotionUpdate()
{
    static unsigned long lastSampleTime = 0;

    unsigned long now = millis();

    if (now - lastSampleTime < 10)
        return;

    lastSampleTime = now;

    if (hall.length() >= HALL_SIZE) {
        broadcastLocomotion();
    }

    if (hall0Changed)
    {
        hall0Changed = false;
        hall += '0';
    }

    if (hall1Changed)
    {
        hall1Changed = false;
        hall += '1';
    }
}

