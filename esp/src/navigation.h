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

  static long int lastTime = 0;

  long int currentTime = millis();

  if (currentTime - lastTime < 10)
    return;

  lastTime = currentTime;

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

      float score = m0 + m1; // better to move forward

      // score based on distance to target
      float distance = clone.getCenter().subtract(targetPosition).length();

      if (distance < 1)
      {
        score += 100;
      }
      else
      {
        score -= distance * 100;
      }

      // front
      if (sonar0Distance < 1 * 5800 && m0 > 0 && m1 > 0) {
        score -= 1000;
      }

      // right
      if (sonar1Distance < 1 * 5800 && m0 > 0 && m1 < 0) {
        score -= 1000;
      }

      // left
      if (sonar2Distance < 1 * 5800 && m0 < 0 && m1 > 0) {
        score -= 1000;
      }

      if (score > bestScore)
      {
        bestScore = score;
        bestM0 = m0;
        bestM1 = m1;
      }
    }
  }

  float distance = odometer.getCenter().subtract(targetPosition).length();

  if (distance < 0.5)
  {
    navigationEnabled = false;
    broadcastData("navigation", "done");
    motor0Speed = 0;
    motor1Speed = 0;
    return;
  }

  motor0Speed = (int8_t)(bestM0 * 25);
  motor1Speed = (int8_t)(bestM1 * 25);
}