#pragma once
#include <Arduino.h>
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
    return avrAckStream.write(str);
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

    return avrAckStream.write(converted);
}

std::shared_ptr<WritePromise> avrClear()
{
    return avrAckStream.write(MODE_CLEAR);
}

typedef Result<uint16_t, ReadError> WordResult;

std::shared_ptr<Promise<WordResult>> avrReadWord()
{

    auto closure = [](Pair<ReadResult, ReadResult> pair)
    {
        if (pair.a.isError() || pair.b.isError())
            return WordResult::fail(pair.a.getError());
        return WordResult((pair.a.getValue() << 8) | pair.b.getValue());
    };

    return avrAckStream.read()->pair(avrAckStream.read())
        ->then<WordResult>(closure);
}

std::shared_ptr<Promise<WordResult>> avrSonar(AvrMode mode)
{
    avrAckStream.write(mode);
    return avrReadWord();
}
