/**
 * @author Wei
 * Navigation code
 */

#pragma once

#include <Arduino.h>

#include <vector>

#include "globals.h"

#define SONAR_TO_M 5800
#define SONAR_THRESHOLD 0.25f

/**
 * Some form of Q-learning with a code based scores and without the learning.
 */
void navigationUpdate()
{
  static bool wasNavigationEnabled = false;

  if (wasNavigationEnabled && !navigationEnabled)
  {
    broadcastData("navigation", "done");
  }

  wasNavigationEnabled = navigationEnabled;

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

  for (int m0 = -5; m0 <= 5; m0++)
  {
    for (int m1 = -5; m1 <= 5; m1++)
    {
      DualOdometer absolute = odometer.clone();
      DualOdometer relative = odometer.clone();

      for (int i = 0; i < 10; i++)
      {
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

      Vec2 difference = targetPosition.clone().subtract(absolute.getCenter());
      Vec2 differenceNorm = difference.normalize();
      float distance = difference.length();
      // score based on distance to target
      score -= distance;

      // score based on angle to target
      score += differenceNorm.dot(absolute.forward());

      float sonar0Score = -(SONAR_TO_M * SONAR_THRESHOLD) / (float)sonar0Distance;
      float sonar1Score = -(SONAR_TO_M * SONAR_THRESHOLD) / (float)sonar1Distance;
      float sonar2Score = -(SONAR_TO_M * SONAR_THRESHOLD) / (float)sonar2Distance;

      Vec2 relativeNorm = relative.getCenter().normalize();

      float sonarScore = 0;
      sonarScore += min(0.0f, sonar0Score * relativeNorm.y);   // front
      sonarScore += min(0.0f, sonar1Score * relativeNorm.x);   // right
      sonarScore += min(0.0f, sonar2Score * -relativeNorm.x);  // left
      // score += sonarScore;

      // punish for going backwards
      score += min(0.0f, relativeNorm.y) * 10;

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
    motor0Speed = 0;
    motor1Speed = 0;
    return;
  }

  motor0Speed = (int8_t)(bestM0 * 10);
  motor1Speed = (int8_t)(bestM1 * 10);
}