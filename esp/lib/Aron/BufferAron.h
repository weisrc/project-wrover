#pragma once
#include "BaseAron.h"

class BufferAron : virtual public Aron
{
protected:
    char *buffer;
    size_t size;

    void add(char c) override
    {
        if (position >= size)
            throw SD_OOM;
        buffer[position] = c;
    }

public:
    BufferAron(char *buffer, size_t size)
    {
        this->buffer = buffer;
        this->size = size;
    }
};
