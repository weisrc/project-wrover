#pragma once
#include <unity.h>

#include "vec2.h"

/**
 * Test Vec2 add method.
 */
void test_vec2_shouldAdd()
{
  Vec2 a(1, 2);
  Vec2 b(3, 4);
  Vec2 c = a.add(b);

  TEST_ASSERT_EQUAL(4, c.x);
  TEST_ASSERT_EQUAL(6, c.y);
}

/**
 * Test Vec2 subtract method.
 */
void test_vec2_shouldSubtract()
{
  Vec2 a(3, 4);
  Vec2 b(1, 2);
  Vec2 c = a.subtract(b);

  TEST_ASSERT_EQUAL(2, c.x);
  TEST_ASSERT_EQUAL(2, c.y);
}

/**
 * Test Vec2 multiply method.
 */
void test_vec2_shouldMultiply()
{
  Vec2 a(1, 2);
  Vec2 b(3, 4);
  Vec2 c = a.multiply(b);

  TEST_ASSERT_EQUAL(3, c.x);
  TEST_ASSERT_EQUAL(8, c.y);
}

/**
 * Test Vec2 translate method.
 */
void test_vec2_shouldTranslate()
{
  Vec2 a(1, 2);
  Vec2 b(3, 4);
  a.translate(b);

  TEST_ASSERT_EQUAL(4, a.x);
  TEST_ASSERT_EQUAL(6, a.y);
}

/**
 * Test Vec2 scale method.
 */
void test_vec2_shouldScale()
{
  Vec2 a(1, 2);
  a.scale(2);

  TEST_ASSERT_EQUAL(2, a.x);
  TEST_ASSERT_EQUAL(4, a.y);
}

/**
 * Test Vec2 normalize method.
 */
void test_vec2_shouldNormalize()
{
  Vec2 a(3, 4);
  Vec2 b = a.normalize();

  TEST_ASSERT_EQUAL(0.6, b.x);
  TEST_ASSERT_EQUAL(0.8, b.y);
}

/**
 * Test Vec2 length squared method.
 */
void test_vec2_shouldLength2()
{
  Vec2 a(3, 4);
  float b = a.length2();

  TEST_ASSERT_EQUAL(25, b);
}

/**
 * Test Vec2 length method.
 */
void test_vec2_shouldLength()
{
  Vec2 a(3, 4);
  float b = a.length();

  TEST_ASSERT_EQUAL(5, b);
}

/**
 * Test Vec2 clone method.
 */
void test_vec2_shouldClone()
{
  Vec2 a(3, 4);
  Vec2 b = a.clone();

  TEST_ASSERT_EQUAL(3, b.x);
  TEST_ASSERT_EQUAL(4, b.y);
}

/**
 * Test Vec2 between method.
 */
void test_vec2_shouldBetween()
{
  Vec2 a(1, 1);
  Vec2 b(3, 3);
  Vec2 c = a.between(b);

  TEST_ASSERT_EQUAL(2, c.x);
  TEST_ASSERT_EQUAL(2, c.y);
}

/**
 * Test Vec2 direction method.
 */
void test_vec2_shouldDirection()
{
  Vec2 a(1, 1);
  float b = a.direction();

  TEST_ASSERT_EQUAL(M_PI_4, b);
}

/**
 * Test Vec2 setDirection method.
 */
void test_vec2_shouldSetDirection()
{
  Vec2 a(2, 0);
  a.setDirection(M_PI_2);

  TEST_ASSERT_EQUAL(0, a.x);
  TEST_ASSERT_EQUAL(2, a.y);
}

/**
 * Test Vec2 rotate method.
 */
void test_vec2_shouldRotate()
{
  Vec2 a(1, 0);
  a.rotate(M_PI_2);

  TEST_ASSERT_EQUAL(0, a.x);
  TEST_ASSERT_EQUAL(1, a.y);
}

/**
 * Test Vec2 zero method.
 */
void test_vec2_shouldZero()
{
  Vec2 a = Vec2::zero();

  TEST_ASSERT_EQUAL(0, a.x);
  TEST_ASSERT_EQUAL(0, a.y);
}

/**
 * Test Vec2 polar method.
 */
void test_vec2_shouldPolar()
{
  Vec2 a = Vec2::polar(M_PI, 1);

  TEST_ASSERT_EQUAL(-1, a.x);
  TEST_ASSERT_EQUAL(0, a.y);
}
