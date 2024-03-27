#pragma once
#include <Arduino.h>

#include "globals.h"
#include "data_utils.h"
#include "avr_serial.h"
#include "channel.h"

int8_t motor0Speed = 0;
int8_t motor1Speed = 0;

#define SONAR_ARRAY_SIZE 5

uint16_t sonar0Average = 0;
uint16_t sonar1Average = 0;
uint16_t sonar2Average = 0;

bool hall0Changed = false;
bool hall1Changed = false;
bool motor0Reverse = false;
bool motor1Reverse = false;
String hall;

void setMotor(Channel &chan, JsonDocument &request)
{
    motor0Speed = request["m0"].as<int8_t>();
    motor1Speed = request["m1"].as<int8_t>();
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

void locomotionReply(Channel &chan)
{
    JsonDocument data;
    data["type"] = "locomotion";
    data["hall"] = hall;
    JsonArray sonars = data["sonar"].to<JsonArray>();
    sonars.add(sonar0Average);
    sonars.add(sonar1Average);
    sonars.add(sonar2Average);
    chan.send(data);
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

void sonarUpdate()
{

    static uint16_t sonar0Array[SONAR_ARRAY_SIZE] = {0};
    static uint16_t sonar1Array[SONAR_ARRAY_SIZE] = {0};
    static uint16_t sonar2Array[SONAR_ARRAY_SIZE] = {0};
    static size_t i = 0;

    static unsigned long lastSampleTime = 0;
    unsigned long now = millis();

    if (now - lastSampleTime < 200)
        return;

    lastSampleTime = now;

    sonar0Array[i] = avrSonar(MODE_SONAR0);
    sonar1Array[i] = avrSonar(MODE_SONAR1);
    sonar2Array[i] = avrSonar(MODE_SONAR2);
    i = (i + 1) % SONAR_ARRAY_SIZE;

    uint32_t sonar0Sum = 0;
    uint32_t sonar1Sum = 0;
    uint32_t sonar2Sum = 0;

    for (size_t j = 0; j < SONAR_ARRAY_SIZE; j++)
    {
        sonar0Sum += sonar0Array[j];
        sonar1Sum += sonar1Array[j];
        sonar2Sum += sonar2Array[j];
    }

    sonar0Average = sonar0Sum / SONAR_ARRAY_SIZE;
    sonar1Average = sonar1Sum / SONAR_ARRAY_SIZE;
    sonar2Average = sonar2Sum / SONAR_ARRAY_SIZE;
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

    if (now - lastSampleTime < 20)
        return;

    lastSampleTime = now;

    if (hall.length() >= HALL_SIZE)
        hall.clear();

    if (hall0Changed)
    {
        odometer.moveLeft(motor0Reverse);
        hall += motor0Reverse ? 'L' : 'l';
    }

    if (hall1Changed)
    {
        odometer.moveRight(motor1Reverse);
        hall += motor1Reverse ? 'R' : 'r';
    }

    hall0Changed = false;
    hall1Changed = false;
}
