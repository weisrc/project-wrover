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
    avrSerial.begin(4800, SWSERIAL_8E1, AVR_RX, AVR_TX);

    if (!avrSerial)
    {
        Serial.println("failed to configure serial");

        while (1)
            delay(1000);
    }
}

int avrRead()
{
    unsigned long startTime = millis();
    while (1)
    {
        if (avrSerial.available())
        {
            char data = avrSerial.read();
            Serial.println("AVR serial rx: " + String(data) + " (" + String((int)data) + ")");
            return data;
        }
        if (millis() - startTime > AVR_SERIAL_TIMEOUT)
        {
            Serial.println("AVR serial rx: Timeout");
            return -1;
        }
    }
}

void avrWrite(char data)
{
    do
    {
        avrSerial.write(data);
        Serial.println("AVR serial tx: " + String(data) + " (" + String((int)data) + ")");
    } while (avrRead() == -1);
}

int avrReadWithAck()
{
    int data = avrRead();
    if (data != -1)
    {
        Serial.println("Sending ack for " + String(data) + " (" + String((int)data) + ")");
        avrSerial.write(MODE_NONE); // any value works
    }
    return data;
}

void avrSend(AvrMode mode, char data)
{
    avrWrite(mode);
    avrWrite(data);
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
    avrWrite(MODE_CLEAR);
}

char avrStaticRead()
{
    static char lastData = 0;
    int got = avrReadWithAck();
    if (got == -1)
        return lastData;
    lastData = got;
    return got;
}

uint16_t avrStaticReadWord()
{
    uint16_t data;
    data = avrStaticRead();
    data <<= 8;
    data += avrStaticRead();
    return data;
}

uint16_t avrSonar(AvrMode mode)
{
    static uint16_t lastData = 0;
    avrWrite(mode);
    uint16_t out = avrStaticReadWord();
    if (out > UINT16_MAX / 2)
        return lastData;
    lastData = out;
    return out;
}
