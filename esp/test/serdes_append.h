#pragma once
#include "unity.h"
#include "serdes.h"

void serdesShouldAppendInt(void)
{
  char data[10] = {};
  Serdes serdes(data);

  TEST_ASSERT_EQUAL_MESSAGE(false, serdes.append(SD_INT), "not ready for SD_INT");
  TEST_ASSERT_EQUAL_MESSAGE(false, serdes.append(4), "not ready for 4");
  TEST_ASSERT_EQUAL_MESSAGE(false, serdes.append(3), "not ready for 3");
  TEST_ASSERT_EQUAL_MESSAGE(false, serdes.append(2), "not ready for 2");
  TEST_ASSERT_EQUAL_MESSAGE(true, serdes.append(1), "ready for 1");

  // int value;

  // serdes.deserialize();
  // serdes.value(value);
  // TEST_ASSERT_EQUAL(0x01020304, value);
  // TEST_ASSERT_EQUAL(5, serdes.size());
}
