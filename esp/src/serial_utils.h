#pragma once
#include <Arduino.h>

void waitForChar(char c)
{
    while (!Serial.available() || Serial.read() != c)
        continue;
}

String promptString(String prompt = "", bool secret = false)
{
    String value = "";

    Serial.print("[INPUT] ");
    Serial.print(prompt);
    Serial.print(": ");

    while (true)
    {
        while (!Serial.available())
            continue;
        char c = Serial.read();

        if (c == '\r')
            continue;
        if (c == '\n')
        {
            Serial.println();
            break;
        }

        if (c == '\b')
        {
            if (value.length())
            {
                Serial.write('\b');
                value.remove(value.length() - 1);
            }
            continue;
        }

        Serial.write(secret ? '*' : c);
        value += c;
    }
    return value;
}

int promptInt(String prompt = "", int from = 1, int to = INT_MAX)
{
    String value = "";
    bool isStart = true;

    Serial.print("[INPUT] ");
    Serial.print(prompt);
    Serial.print(": ");

    while (true)
    {
        while (!Serial.available())
            continue;
        char c = Serial.read();

        if (c == '\n')
        {
            Serial.println();
            break;
        }

        if (c == '\b' && value.length())
        {
            Serial.write(c);
            value.remove(value.length() - 1);
            continue;
        }

        if (c < '0' || c > '9')
            continue;

        if (isStart && c == '0')
            continue;

        value += c;

        int next = atoi(value.c_str());
        if (next >= from && next <= to)
        {
            Serial.write(c);
            isStart = false;
        }
        else
        {
            value.remove(value.length() - 1);
        }
    }
    return atoi(value.c_str());
}