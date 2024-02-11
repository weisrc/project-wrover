#pragma once
#include "unity.h"
#include "Aron.h"

void aronShouldSerializeInt(void)
{
  char data[10];
  int value = 123;
  BufferSerialize aron(data, 10);
  aron.number(value);
  TEST_ASSERT_EQUAL_STRING("123", data);
}
