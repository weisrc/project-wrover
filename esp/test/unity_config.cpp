#include "Arduino.h"
#include "unity.h"

#include "aron_serialize.h"

void setup()
{
    delay(2000);
    UNITY_BEGIN();
    
    RUN_TEST(aronShouldSerializeInt);
    
    UNITY_END();
}

void loop() {}
