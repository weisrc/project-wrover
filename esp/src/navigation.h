/**
 * @author Wei
 * Navigation code
 */

#pragma once

#include <Arduino.h>

#include <vector>

#include "globals.h"

#define SONAR_TO_M 5800

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
      DualOdometer absolute = odometer.clone();
      DualOdometer relative = odometer.clone();

      for (int i = 0; i < 5; i++) {
        int absM0 = abs(m0);
        int absM1 = abs(m1);

        while (absM0 | absM1)
        {
          if (absM0)
          {
            bool backwards = m0 < 0;
            absolute.moveLeft(backwards);
            relative.moveLeft(backwards);
            absM0--;
          }

          if (absM1)
          {
            bool backwards = m1 < 0;
            absolute.moveRight(backwards);
            relative.moveRight(backwards);
            absM1--;
          }
        }
      }

      float score = 0;

      float distance = absolute.getCenter().subtract(targetPosition).length();
      // score based on distance to target
      score -= distance;

      float sonar0Score = -SONAR_TO_M / (float)sonar0Distance;
      float sonar1Score = -SONAR_TO_M / (float)sonar1Distance;
      float sonar2Score = -SONAR_TO_M / (float)sonar2Distance;

      Vec2 norm = relative.getCenter().normalize();

      float sonarScore = 0;
      sonarScore += min(0.0f, sonar0Score * norm.y); // front
      sonarScore += min(0.0f, sonar1Score * norm.x); // right
      sonarScore += min(0.0f, sonar2Score * -norm.x); // left
      
      score += sonarScore;

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