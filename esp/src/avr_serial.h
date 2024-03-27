#pragma once
#include "globals.h"

enum AvrMode
{
    MODE_NONE = 0,
    MODE_SONAR0 = 1,
    MODE_SONAR1 = 2,
    MODE_SONAR2 = 3,
    MODE_MOTOR0 = 4,
    MODE_MOTOR1 = 5,
    MODE_WRITE = 6,
    MODE_COMMAND = 7,
    MODE_CLEAR = 8,
    MODE_LAST = 9
};

void avrSerialSetup()
{
    avrSerial.begin(2400, SWSERIAL_8N1, AVR_RX, AVR_TX);

    if (!avrSerial)
    {
        Serial.println("failed to configure serial");

        while (1)
            delay(1000);
    }
}

void avrSend(AvrMode mode, char data)
{
    avrSerial.write(mode);
    avrSerial.write(data);
}

void avrLCDSecond()
{
    avrSend(MODE_COMMAND, 0x80 + 0x40);
}

void avrPrint(String str)
{
    for (char c : str)
    {
        if (c == '\n')
            avrLCDSecond();
        else
            avrSend(MODE_WRITE, c);
    }
}

void avrClear()
{
    avrSerial.write(MODE_CLEAR);
}

char avrReadByte()
{
    static char lastData = 0;
    unsigned long startTime = millis();
    while (1) {
        if (avrSerial.available()) {
            lastData = avrSerial.read();
            return lastData;
        }
        if (millis() - startTime > AVR_SERIAL_TIMEOUT) {
            return lastData;
        }
    }
}

uint16_t avrReadWord()
{
    uint16_t data;
    data = avrReadByte();
    data <<= 8;
    data += avrReadByte();
    return data;
}

uint16_t avrSonar(AvrMode mode)
{
    static uint16_t lastData = 0;
    avrSerial.write(mode);
    uint16_t out = avrReadWord();
    if (out > UINT16_MAX / 2)
        return lastData;
    lastData = out;
    return out;
}
