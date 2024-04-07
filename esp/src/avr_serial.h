#pragma once
#include <Arduino.h>
#include "globals.h"

enum AvrMode
{
    MODE_NONE = 100,
    MODE_SONAR0 = 101,
    MODE_SONAR1 = 102,
    MODE_SONAR2 = 103,
    MODE_MOTOR0 = 104,
    MODE_MOTOR1 = 105,
    MODE_WRITE = 106,
    MODE_COMMAND = 107,
    MODE_CLEAR = 108,
    MODE_LAST = 109
};

void avrSerialSetup()
{
    avrSerialBase.begin(4800, SWSERIAL_8O1, AVR_RX, AVR_TX);

    if (!avrSerialBase)
    {
        LOG_ERROR("failed to configure AVR serial");
        while (1)
            delay(1000);
    }
}

std::shared_ptr<WritePromise> avrSend(AvrMode mode, char data)
{
    String str;
    str += (char)mode;
    str += data;
    return avrSerial.write(str);
}

std::shared_ptr<WritePromise> avrLCDSecond()
{
    return avrSend(MODE_COMMAND, 0x80 + 0x40);
}

std::shared_ptr<WritePromise> avrPrint(String str)
{
    String converted;

    for (int i = 0; i < str.length(); i++)
    {
        char c = str[i];
        if (c == '\n')
        {
            converted += (char)MODE_COMMAND;
            converted += (char)(0x80 + 0x40);
        }
        else
        {
            converted += (char)MODE_WRITE;
            converted += c;
        }
    }

    return avrSerial.write(converted);
}

std::shared_ptr<WritePromise> avrClear()
{
    return avrSerial.write(MODE_CLEAR);
}

typedef Result<uint16_t, AsyncSerialError> WordResult;

std::shared_ptr<Promise<WordResult>> avrReadWord()
{

    auto closure = [](Pair<ReadResult, ReadResult> pair)
    {
        if (pair.a.isError() || pair.b.isError())
            return WordResult::fail(pair.a.getError());
        return WordResult((pair.a.getValue() << 8) | pair.b.getValue());
    };

    return avrSerial.read()->pair(avrSerial.read())
        ->then<WordResult>(closure);
}

std::shared_ptr<Promise<WordResult>> avrSonar(AvrMode mode)
{
    avrSerial.write(mode);
    return avrReadWord();
}
