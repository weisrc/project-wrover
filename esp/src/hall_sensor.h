#pragma once
#include <Arduino.h>

#include "globals.h"
#include "data_utils.h"

bool hall0Changed = false;
bool hall1Changed = false;

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
}

void hallSensorCheck()
{
    static String data;
    static unsigned long lastTime = 0;

    if (hall0Changed)
    {
        hall0Changed = false;
        data += '0';
    }

    if (hall1Changed)
    {
        hall1Changed = false;
        data += '1';
    }

    unsigned long now = millis();
    if (now - lastTime < 500)
        return;

    lastTime = now;

    if (data.length() == 0)
        return;

    broadcastData("hall", data);
    data.clear();
}