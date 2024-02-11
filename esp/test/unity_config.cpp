#include "Arduino.h"
#include "unity.h"

#include "serdes_deserialize.h"
#include "serdes_serialize.h"
#include "serdes_append.h"

void setup()
{
    delay(2000);
    UNITY_BEGIN();
    RUN_TEST(serdesShouldSerializeChar);
    RUN_TEST(serdesShouldSerializeInt);
    RUN_TEST(serdesShouldSerializeString);
    RUN_TEST(serdesShouldSerializeStringArray);
    
    RUN_TEST(serdesShouldDeserializeChar);
    RUN_TEST(serdesShouldDeserializeInt);
    RUN_TEST(serdesShouldDeserializeString);
    RUN_TEST(serdesShouldDeserializeStringArray);
    
    RUN_TEST(serdesShouldAppendInt);
    RUN_TEST(serdesShouldAppendString);
    RUN_TEST(serdesShouldAppendStringArray);

    UNITY_END();
}

void loop() {}
