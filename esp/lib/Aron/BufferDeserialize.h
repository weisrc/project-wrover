#pragma once
#include "Deserialize.h"
#include "BufferAron.h"

class BufferDeserialize : public Deserializer, public BufferAron
{
    using BufferAron::BufferAron;
};