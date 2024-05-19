/**
 * @author Wei
 * Navigation code
 */

#pragma once

#include <Arduino.h>

#include <vector>

#include "globals.h"

#define M_TO_SONAR 5800
#define SONAR_THRESHOLD 1.0f

float getSonarScore(Vec2 norm)
{
  float sonar0Score = -(M_TO_SONAR * SONAR_THRESHOLD) / (float)sonar0Distance;
  float sonar1Score = -(M_TO_SONAR * SONAR_THRESHOLD) / (float)sonar1Distance;
  float sonar2Score = -(M_TO_SONAR * SONAR_THRESHOLD) / (float)sonar2Distance;

  float sonarScore = 0;
  sonarScore += min(0.0f, sonar0Score * norm.y);   // front
  sonarScore += min(0.0f, sonar1Score * norm.x);   // right
  sonarScore += min(0.0f, sonar2Score * -norm.x);  // left
  return sonarScore;
}

void navigateTowards(Vec2 towardsPosition)
{
  int bestM0 = 0;
  int bestM1 = 0;
  float bestScore = -INFINITY;

  for (int m0 = -5; m0 <= 5; m0++)
  {
    for (int m1 = -5; m1 <= 5; m1++)
    {
      DualOdometer absolute = odometer.clone();
      DualOdometer relative = odometer.clone();

      if (abs(m0) + abs(m1) < 3) {
        continue;
      }

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

      Vec2 difference = towardsPosition.clone().subtract(absolute.getCenter());
      Vec2 differenceNorm = difference.clone().normalize();
      float distance = difference.length();
      // score based on distance to target
      // Serial.println("Distance: " + String(distance));
      score -= distance;

      // score based on angle to target
      float alignment = differenceNorm.dot(absolute.forward());
      // Serial.println("Alignment: " + String(alignment));
      score += alignment;

      Vec2 offset = relative.getCenter();
      score += offset.length();
      Vec2 relativeNorm = offset.clone().normalize();

      score += getSonarScore(relativeNorm);      

      if (score > bestScore)
      {
        bestScore = score;
        bestM0 = m0;
        bestM1 = m1;
      }
    }
  }

  motor0Speed = (int8_t)(bestM0 * 10);
  motor1Speed = (int8_t)(bestM1 * 10);
}

/**
 * Some form of Q-learning with a code based scores and without the learning.
 */
void navigationUpdate()
{
  static bool wasNavigationEnabled = false;

  const bool navigationEnabled = navigationMode != NavigationMode::OFF;
  static NavigationMode lastNavigationMode = NavigationMode::OFF;

  if (lastNavigationMode != navigationMode) {
    Serial.println("navigation mode changed: " + String(navigationMode));
  }

  lastNavigationMode = navigationMode;

  if (wasNavigationEnabled && !navigationEnabled)
  {
    broadcastData("navigation", "done");
  }

  wasNavigationEnabled = navigationEnabled;

  if (!navigationEnabled)
    return;

  static long int lastTime = 0;

  long int currentTime = millis();

  if (currentTime - lastTime < 100)
    return;

  lastTime = currentTime;

  Vec2 currentPosition = odometer.getCenter();
  Vec2 difference = targetPosition.clone().subtract(currentPosition);
  float direction = difference.direction();
  float distance = difference.length();

  switch (navigationMode) {
    case NavigationMode::DIRECT:
    {
      navigateTowards(targetPosition);
      if (sonar0Distance < 1 * M_TO_SONAR) {
        if (sonar2Distance > sonar1Distance) {
          navigationMode = NavigationMode::DETOUR_LEFT;
        } else {
          navigationMode = NavigationMode::DETOUR_RIGHT;
        }
      }
      break;
    }
    case NavigationMode::DETOUR_LEFT:
    {
      navigateTowards(Vec2::polar(direction - M_PI_2, 10).add(currentPosition));
      if (sonar1Distance > 1 * M_TO_SONAR) {
        navigationMode = NavigationMode::DIRECT;
      }
      break;
    }
    case NavigationMode::DETOUR_RIGHT:
    {
      navigateTowards(Vec2::polar(direction + M_PI_2, 10).add(currentPosition));
      if (sonar2Distance > 1 * M_TO_SONAR) {
        navigationMode = NavigationMode::DIRECT;
      }
      break;
    }
  }

  if (distance < 0.5)
  {
    navigationMode = NavigationMode::OFF;
    motor0Speed = 0;
    motor1Speed = 0;
    return;
  }
}