#include <Arduino.h>
#include <unity.h>

#include "promise_test.h"
#include "vec2_test.h"
#include "avr_serial_test.h"

void setup()
{
  delay(2000);
  UNITY_BEGIN();

  RUN_TEST(test_promise_finallyShouldBeCalled);
  RUN_TEST(test_promise_thenShouldChain);
  RUN_TEST(test_promise_pairShouldCombine);
  RUN_TEST(test_promise_raceShouldCombine);

  RUN_TEST(test_vec2_shouldAdd);
  RUN_TEST(test_vec2_shouldTranslate);
  RUN_TEST(test_vec2_shouldScale);
  RUN_TEST(test_vec2_shouldNormalize);
  RUN_TEST(test_vec2_shouldLength2);
  RUN_TEST(test_vec2_shouldLength);
  RUN_TEST(test_vec2_shouldClone);
  RUN_TEST(test_vec2_shouldSubtract);
  RUN_TEST(test_vec2_shouldMultiply);
  RUN_TEST(test_vec2_shouldPolar);
  RUN_TEST(test_vec2_shouldRotate);
  RUN_TEST(test_vec2_shouldSetDirection);
  RUN_TEST(test_vec2_shouldZero);

  RUN_TEST(test_avr_serial_shouldGetSonarReadings);

  UNITY_END();
}

void loop() {}
