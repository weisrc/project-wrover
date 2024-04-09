#pragma once
#include <unity.h>

#include "avr_serial.h"
#include "globals.h"

/**
 * Test if the avr serial can get readings from the sonar sensor
 */
void test_avr_serial_shouldGetSonarReadings()
{
  volatile bool done = false;
  unsigned long start = millis();

  avrSerialSetup();

  // register a listener for the sonar readings
  avrSonar(MODE_SONAR0)
      ->finally(
          [&](WordResult res)
          {
            done = true;
            TEST_ASSERT_TRUE(res.isOk());  // check if the result is ok
          });

  while (!done)
  {
    if (millis() - start > 1000)  // timeout after 1 second
    {
      TEST_FAIL_MESSAGE("Sonar serial timeout");
      break;
    }
    avrSerial.update();
  }
}
