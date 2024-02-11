#pragma once
#include "unity.h"
#include "serdes.h"

void serdesShouldDeserializeChar(void)
{
  char data[10] = {SD_BYTE, 'c'};
  char value = 'a';
  Serdes serdes(data, 10);
  serdes.deserialize();
  serdes.value(value);
  TEST_ASSERT_EQUAL('c', value);
  TEST_ASSERT_EQUAL(2, serdes.size());
  TEST_ASSERT_EQUAL(true, serdes.ok());
}

void serdesShouldDeserializeInt(void)
{
  char data[10] = {SD_INT, 0x04, 0x03, 0x02, 0x01};
  int value = 0;
  Serdes serdes(data, 10);
  serdes.deserialize();
  serdes.value(value);
  TEST_ASSERT_EQUAL(0x01020304, value);
  TEST_ASSERT_EQUAL(5, serdes.size());
  TEST_ASSERT_EQUAL(true, serdes.ok());
}

void serdesShouldDeserializeString(void)
{
  char data[10] = {SD_START, 'h', 'e', 'l', 'l', 'o', SD_STOP};
  char *value = "this should be overwritten by deserialize()";
  Serdes serdes(data, 10);
  serdes.deserialize();
  serdes.string(value);
  TEST_ASSERT_EQUAL_STRING("hello", value);
  TEST_ASSERT_EQUAL(7, serdes.size());
  TEST_ASSERT_EQUAL(true, serdes.ok());
}

void serdesShouldDeserializeStringArray(void)
{
  char data[32] = {SD_START, SD_START, 'h', 'i', SD_STOP, SD_START, 'b', 'o', 'b', SD_STOP, SD_STOP};
  Serdes serdes(data, 32);

  serdes.deserialize();
  serdes.start();

  char *value1;
  serdes.string(value1);
  TEST_ASSERT_EQUAL_STRING("hi", value1);
  TEST_ASSERT_EQUAL_MESSAGE(false, serdes.stop(), "value1 stop should be false");
  TEST_ASSERT_EQUAL(5, serdes.size());
  TEST_ASSERT_EQUAL_MESSAGE(true, serdes.ok(), "value1 ok should be true");

  char *value2;
  serdes.string(value2);
  TEST_ASSERT_EQUAL_STRING("bob", value2);
  TEST_ASSERT_EQUAL_MESSAGE(true, serdes.stop(), "value2 stop should be true");
  TEST_ASSERT_EQUAL(11, serdes.size());
  TEST_ASSERT_EQUAL_MESSAGE(true, serdes.ok(), "value2 ok should be true");

  TEST_ASSERT_EQUAL_MESSAGE(true, serdes.stop(), "stop at end should be true");
}
