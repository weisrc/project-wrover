#pragma once
#include "globals.h"

void avrSerialSetup()
{
    avrSerial.begin(4800, SWSERIAL_8N1, AVR_RX, AVR_TX);

    if (!avrSerial)
    {
        Serial.println("failed to configure serial");

        while (1)
        {
            delay(1000);
        }
    }
}