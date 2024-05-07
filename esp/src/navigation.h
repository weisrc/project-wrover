/**
 * @author Wei
 * Navigation code
 */

#pragma once

#include <Arduino.h>

#include <vector>

#include "globals.h"

/**
 * Some form of Q-learning with a code based scores and without the learning.
 */
void navigationUpdate()
{
  if (!navigationEnabled)
    return;

  int bestM0 = 0;
  int bestM1 = 0;
  float bestScore = -INFINITY;

  for (int m0 = -2; m0 <= 2; m0++)
  {
    for (int m1 = -2; m1 <= 2; m1++)
    {
      DualOdometer clone = odometer.clone();

      for (int i = 0; i < 5; i++) {
        int absM0 = abs(m0);
        int absM1 = abs(m1);

        while (absM0 | absM1)
        {
          if (absM0)
          {
            clone.moveLeft(m0 < 0);
            absM0--;
          }

          if (absM1)
          {
            clone.moveRight(m1 < 0);
            absM1--;
          }
        }
      }

      float score = 0;

      // score based on distance to target
      float distance = clone.getCenter().subtract(targetPosition).length();

      if (distance < 0.1)
      {
        score += 100;
      }
      else
      {
        score -= distance;
      }

      if (score > bestScore)
      {
        bestScore = score;
        bestM0 = m0;
        bestM1 = m1;
      }
    }
  }

  motor0Speed = (int8_t)(bestM0 * 25);
  motor1Speed = (int8_t)(bestM1 * 25);
}