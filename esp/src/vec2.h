#pragma once
#include <cmath>

class Vec2
{
public:
  float x;
  float y;

  Vec2()
  {
    x = 0;
    y = 0;
  }

  Vec2(float x, float y)
  {
    this->x = x;
    this->y = y;
  }

  Vec2 translate(Vec2 other)
  {
    x += other.x;
    y += other.y;
    return *this;
  }

  Vec2 scale(float scalor)
  {
    x *= scalor;
    y *= scalor;
    return *this;
  }

  Vec2 normalize()
  {
    return scale(1 / length());
  }

  float length2()
  {
    return x * x + y * y;
  }

  float length()
  {
    return sqrt(length2());
  }

  Vec2 clone()
  {
    return Vec2(x, y);
  }

  Vec2 add(Vec2 other)
  {
    return Vec2(x + other.x, y + other.y);
  }

  Vec2 subtract(Vec2 other)
  {
    return Vec2(x - other.x, y - other.y);
  }

  Vec2 multiply(Vec2 other)
  {
    return Vec2(x * other.x, y * other.y);
  }

  Vec2 between(Vec2 other)
  {
    const float x = this->x + other.x;
    const float y = this->y + other.y;
    return Vec2(x / 2, y / 2);
  }

  float direction()
  {
    return atan2(y, x);
  }

  void setDirection(float rad)
  {
    const float length = this->length();
    x = cos(rad) * length;
    y = sin(rad) * length;
  }

  void rotate(float rad)
  {
    setDirection(direction() + rad);
  }

  static Vec2 zero()
  {
    return Vec2(0, 0);
  }

  static Vec2 polar(float rad, float length)
  {
    return Vec2(cos(rad) * length, sin(rad) * length);
  }
};