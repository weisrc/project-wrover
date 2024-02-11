#pragma once
#include "unity.h"
#include "serdes.h"

void serdesShouldAppendInt(void)
{
  char data[10] = {};
  Serdes serdes(data, 10);
  serdes.deserialize();

  TEST_ASSERT_EQUAL_MESSAGE(false, serdes.append(SD_INT), "not ready for SD_INT");
  TEST_ASSERT_EQUAL_MESSAGE(false, serdes.append(4), "not ready for 4");
  TEST_ASSERT_EQUAL_MESSAGE(false, serdes.append(3), "not ready for 3");
  TEST_ASSERT_EQUAL_MESSAGE(false, serdes.append(2), "not ready for 2");
  TEST_ASSERT_EQUAL_MESSAGE(true, serdes.append(1), "ready for 1");

  int value;
  serdes.value(value);
  TEST_ASSERT_EQUAL(0x01020304, value);
  TEST_ASSERT_EQUAL(5, serdes.size());
}

void serdesShouldAppendString(void)
{
  char data[10] = {};
  Serdes serdes(data, 10);
  serdes.deserialize();

  TEST_ASSERT_EQUAL_MESSAGE(false, serdes.append(SD_START), "not ready for SD_START");
  TEST_ASSERT_EQUAL_MESSAGE(false, serdes.append('h'), "not ready for 'h'");
  TEST_ASSERT_EQUAL_MESSAGE(false, serdes.append('e'), "not ready for 'e'");
  TEST_ASSERT_EQUAL_MESSAGE(false, serdes.append('l'), "not ready for 'l'");
  TEST_ASSERT_EQUAL_MESSAGE(false, serdes.append('l'), "not ready for 'l'");
  TEST_ASSERT_EQUAL_MESSAGE(false, serdes.append('o'), "not ready for 'o'");
  TEST_ASSERT_EQUAL_MESSAGE(true, serdes.append(SD_STOP), "ready for SD_STOP");

  char* value;
  serdes.string(value);
  TEST_ASSERT_EQUAL_STRING("hello", value);
  TEST_ASSERT_EQUAL(7, serdes.size());
}

void serdesShouldAppendStringArray(void)
{
  char data[16] = {};
  Serdes serdes(data, 16);
  serdes.deserialize();

  TEST_ASSERT_EQUAL_MESSAGE(false, serdes.append(SD_START), "not ready for SD_START container");
  TEST_ASSERT_EQUAL_MESSAGE(false, serdes.append(SD_START), "not ready for SD_START hello");
  TEST_ASSERT_EQUAL_MESSAGE(false, serdes.append('h'), "not ready for 'h'");
  TEST_ASSERT_EQUAL_MESSAGE(false, serdes.append('i'), "not ready for 'i'");
  TEST_ASSERT_EQUAL_MESSAGE(false, serdes.append(SD_STOP), "not ready for SD_STOP hello");
  TEST_ASSERT_EQUAL_MESSAGE(false, serdes.append(SD_START), "not ready for SD_START world");
  TEST_ASSERT_EQUAL_MESSAGE(false, serdes.append('w'), "not ready for 'w'");
  TEST_ASSERT_EQUAL_MESSAGE(false, serdes.append('o'), "not ready for 'o'");
  TEST_ASSERT_EQUAL_MESSAGE(false, serdes.append('r'), "not ready for 'r'");
  TEST_ASSERT_EQUAL_MESSAGE(false, serdes.append('l'), "not ready for 'l'");
  TEST_ASSERT_EQUAL_MESSAGE(false, serdes.append('d'), "not ready for 'd'");
  TEST_ASSERT_EQUAL_MESSAGE(false, serdes.append(SD_STOP), "ready for SD_STOP world");
  TEST_ASSERT_EQUAL_MESSAGE(true, serdes.append(SD_STOP), "ready for SD_STOP container");

  char* value[4] = {};
  int i;

  serdes.start();
  for (i = 0; !serdes.stop() && i < 4; i++)
  {
    serdes.string(value[i]);
  }

  TEST_ASSERT_EQUAL_MESSAGE(2, i, "iterator should be 2");
  TEST_ASSERT_EQUAL_STRING("hi", value[0]);
  TEST_ASSERT_EQUAL_STRING("world", value[1]);
  TEST_ASSERT_EQUAL_MESSAGE(true, serdes.ok(), "should be ok");
  TEST_ASSERT_EQUAL_MESSAGE(13, serdes.size(), "size should be 13");
}