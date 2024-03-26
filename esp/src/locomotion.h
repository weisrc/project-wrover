#pragma once
#include <Arduino.h>

#include "globals.h"
#include "data_utils.h"
#include "avr_serial.h"

int8_t motor0Speed = 0;
int8_t motor1Speed = 0;

bool hall0Changed = false;
bool hall1Changed = false;
bool motor0Reverse = false;
bool motor1Reverse = false;
String hall;

void setMotor(Channel &chan, JsonDocument &request)
{
    motor0Speed = request["m0"].as<int8_t>();
    motor1Speed = request["m1"].as<int8_t>();
    // avrSend(MODE_MOTOR0, motor0Speed);
    // avrSend(MODE_MOTOR1, motor1Speed);
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

int8_t shiftTowards(int8_t current, int8_t target)
{
    if (current < target)
        return current + 1;
    else if (current > target)
        return current - 1;
    return current;
}

void motorUpdate()
{
    static unsigned long lastSampleTime = 0;
    static int8_t motor0SmoothSpeed = 0;
    static int8_t motor1SmoothSpeed = 0;

    unsigned long now = millis();

    if (now - lastSampleTime < 50)
        return;

    if (motor0SmoothSpeed != motor0Speed)
    {
        motor0SmoothSpeed = shiftTowards(motor0SmoothSpeed, motor0Speed);
        motor0Reverse = motor0SmoothSpeed < 0;
        avrSend(MODE_MOTOR0, motor0SmoothSpeed);
    }

    if (motor1SmoothSpeed != motor1Speed)
    {
        motor1SmoothSpeed = shiftTowards(motor1SmoothSpeed, motor1Speed);
        motor1Reverse = motor1SmoothSpeed < 0;
        avrSend(MODE_MOTOR1, motor1SmoothSpeed);
    }
}

void locomotionUpdate()
{
    static unsigned long lastSampleTime = 0;
    unsigned long now = millis();

    if (locomotionRequested)
    {
        locomotionRequested = false;
        broadcastLocomotion();
    }

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
