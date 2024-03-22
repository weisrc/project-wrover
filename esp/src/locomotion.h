#pragma once
#include <Arduino.h>

#include "globals.h"
#include "data_utils.h"
#include "avr_serial.h"

bool hall0Changed = false;
bool hall1Changed = false;
bool motor0Reverse = false;
bool motor1Reverse = false;
String hall;

void setMotor(Channel &chan, JsonDocument &request)
{
    int8_t m0 = request["m0"].as<int8_t>();
    int8_t m1 = request["m1"].as<int8_t>();
    if (m0 > 0)
        motor0Reverse = false;
    else if (m0 < 0)
        motor0Reverse = true;

    if (m1 > 0)
        motor1Reverse = false;
    else if (m1 < 0)
        motor1Reverse = true;
    avrSend(MODE_MOTOR0, m0);
    avrSend(MODE_MOTOR1, m1);
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

void broadcastLocomotion()
{
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

    if (now - lastSampleTime < 20)
        return;

    lastSampleTime = now;

    if (hall.length() >= HALL_SIZE)
        broadcastLocomotion();

    if (hall0Changed)
        hall += motor0Reverse ? 'L' : 'l';

    if (hall1Changed)
        hall += motor1Reverse ? 'R' : 'r';

    hall0Changed = false;
    hall1Changed = false;
}
