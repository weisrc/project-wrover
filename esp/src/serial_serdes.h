#pragma once
#include "serdes.h"

class SerialSerdes: public Serdes
{
    void write(char c)
    {
        Serdes::write(c);
    }
};