#pragma once
#include <Arduino.h>

#include "globals.h"
#include "data_utils.h"
#include "avr_serial.h"

bool hall0Changed = false;
bool hall1Changed = false;
bool willMotor0Reverse = false;
bool willMotor1Reverse = false;
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
    static unsigned long motor0ChangedAt = 0;
    static unsigned long motor1ChangedAt = 0;

    unsigned long now = millis();

    if (now - lastSampleTime < 20)
        return;

    lastSampleTime = now;

    if (hall.length() >= HALL_SIZE)
        broadcastLocomotion();

    if (hall0Changed)
    {
        motor0ChangedAt = now;
        hall += willMotor0Reverse ? 'L' : 'l';
    }

    if (hall1Changed)
    {
        motor1ChangedAt = now;
        hall += willMotor1Reverse ? 'R' : 'r';
    }

    if (now - motor0ChangedAt > 50)
        willMotor0Reverse = motor0Reverse;

    if (now - motor1ChangedAt > 50)
        willMotor1Reverse = motor1Reverse;

    hall0Changed = false;
    hall1Changed = false;
}
