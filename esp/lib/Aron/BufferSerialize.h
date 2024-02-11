#pragma once
#include "Serialize.h"
#include "BufferAron.h"

class BufferSerialize : public Serializer, public BufferAron
{
    using BufferAron::BufferAron;
};