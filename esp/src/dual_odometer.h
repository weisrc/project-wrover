/**
 * @author Wei
 * Dual odometer class
 */

#pragma once
#include "vec2.h"

class DualOdometer
{
public:
  float radius;
  float delta;

  Vec2 left;
  Vec2 right;

  /**
   * Initialize the dual odometer
   * @param radius the distance from a wheel to the center
   * @param delta how many radians to move with the opposite wheel as the pivot
   */
  DualOdometer(float radius, float delta)
  {
    this->radius = radius;
    this->delta = delta;
    this->left = Vec2(-radius, 0);
    this->right = Vec2(radius, 0);
  }

  void moveLeft(bool backwards = false)
  {
    int sign = backwards ? -1 : 1;
    float angle = this->left.subtract(this->right).direction();
    this->left = Vec2::polar(angle + this->delta * sign, this->radius * 2).add(this->right);
  }

  void moveRight(bool backwards = false)
  {
    int sign = backwards ? -1 : 1;
    float angle = this->right.subtract(this->left).direction();
    this->right = Vec2::polar(angle - this->delta * sign, this->radius * 2).add(this->left);
  }

  float getDirection()
  {
    return this->right.subtract(this->left).direction();
  }

  Vec2 getCenter()
  {
    return this->left.between(this->right);
  }

  void setPosition(Vec2 center, float direction)
  {
    Vec2 offset = Vec2::polar(direction, this->radius + M_PI_2);
    this->left = center.subtract(offset);
    this->right = center.add(offset);
  }
};