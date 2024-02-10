#pragma once
#include "unity.h"
#include "serdes.h"

void serdesShouldSerializeChar(void)
{
  char data[10];
  char value = 'a';
  Serdes serdes(data);
  serdes.serialize();
  serdes.value(value);
  TEST_ASSERT_EQUAL(SD_BYTE, data[0]);
  TEST_ASSERT_EQUAL('a', data[1]);
  TEST_ASSERT_EQUAL(2, serdes.size());
}

void serdesShouldSerializeInt(void)
{
  char data[10];
  int value = 0x01020304;
  Serdes serdes(data);
  serdes.serialize();
  serdes.value(value);
  TEST_ASSERT_EQUAL(SD_INT, data[0]);
  TEST_ASSERT_EQUAL(0x04, data[1]);
  TEST_ASSERT_EQUAL(0x03, data[2]);
  TEST_ASSERT_EQUAL(0x02, data[3]);
  TEST_ASSERT_EQUAL(0x01, data[4]);
  TEST_ASSERT_EQUAL(5, serdes.size());
}

void serdesShouldSerializeString(void)
{
  char data[10];
  char *value = "hello";
  Serdes serdes(data);
  serdes.serialize();
  serdes.string(value);
  TEST_ASSERT_EQUAL(SD_START, data[0]);
  TEST_ASSERT_EQUAL('h', data[1]);
  TEST_ASSERT_EQUAL('e', data[2]);
  TEST_ASSERT_EQUAL('l', data[3]);
  TEST_ASSERT_EQUAL('l', data[4]);
  TEST_ASSERT_EQUAL('o', data[5]);
  TEST_ASSERT_EQUAL(SD_STOP, data[6]);
  TEST_ASSERT_EQUAL(7, serdes.size());
}

void serdesShouldSerializeStringArray(void)
{
  char data[32];
  char *value[] = {"hi", "bob"};
  Serdes serdes(data);
  serdes.serialize();
  serdes.start();
  for (int i = 0; i < 2; i++)
  {
    serdes.string(value[i]);
  }
  serdes.stop();

  TEST_ASSERT_EQUAL(SD_START, data[0]);
  TEST_ASSERT_EQUAL(SD_START, data[1]);
  TEST_ASSERT_EQUAL('h', data[2]);
  TEST_ASSERT_EQUAL('i', data[3]);
  TEST_ASSERT_EQUAL(SD_STOP, data[4]);
  TEST_ASSERT_EQUAL(SD_START, data[5]);
  TEST_ASSERT_EQUAL('b', data[6]);
  TEST_ASSERT_EQUAL('o', data[7]);
  TEST_ASSERT_EQUAL('b', data[8]);
  TEST_ASSERT_EQUAL(SD_STOP, data[9]);
  TEST_ASSERT_EQUAL(SD_STOP, data[10]);
  TEST_ASSERT_EQUAL(11, serdes.size());
}

