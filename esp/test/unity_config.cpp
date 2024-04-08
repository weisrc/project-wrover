#include <Arduino.h>
#include <unity.h>

#include "promise_test.h"

void setup()
{
  delay(2000);
  UNITY_BEGIN();

  RUN_TEST(test_promise_finallyShouldBeCalled);
  RUN_TEST(test_promise_thenShouldChain);
  RUN_TEST(test_promise_pairShouldCombine);
  RUN_TEST(test_promise_raceShouldCombine);

  UNITY_END();
}

void loop() {}
