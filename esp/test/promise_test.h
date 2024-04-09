#pragma once
#include <unity.h>

#include "promise.h"

/**
 * Test if the `finally` method is called when the promise is resolved.
 */
void test_promise_finallyShouldBeCalled()
{
  volatile bool thenCalled = false;
  volatile int thenValue = 0;
  Promise<int> promise;

  promise.finally(
      [&](int value)
      {
        thenCalled = true;
        thenValue = value;
      });

  promise.resolve(123);

  TEST_ASSERT_TRUE(thenCalled);
  TEST_ASSERT_EQUAL(123, thenValue);
}

/**
 * Test if the `then` method can remap the value of the resolved promise.
 */
void test_promise_thenShouldChain()
{
  volatile bool thenCalled = false;
  volatile int thenValue = 0;
  Promise<int> promise;

  promise.then<int>([&](int a) { return a * 2; })
      ->finally(
          [&](int value)
          {
            thenCalled = true;
            thenValue = value;
          });

  promise.resolve(123);

  TEST_ASSERT_TRUE(thenCalled);
  TEST_ASSERT_EQUAL(246, thenValue);
}

/**
 * Test if the `pair` method can combine two promises into one.
 */
void test_promise_pairShouldCombine()
{
  volatile bool thenCalled = false;
  volatile int thenValue = 0;
  auto promiseA = std::make_shared<Promise<int>>();
  auto promiseB = std::make_shared<Promise<int>>();

  promiseA->pair(promiseB)->finally(
      [&](Pair<int, int> pair)
      {
        thenCalled = true;
        thenValue = pair.a + pair.b;
      });

  promiseA->resolve(123);

  TEST_ASSERT_FALSE(thenCalled);

  promiseB->resolve(123);

  TEST_ASSERT_TRUE(thenCalled);
  TEST_ASSERT_EQUAL(246, thenValue);
}

/**
 * Test if the `race` method can combine two promises into one.
 */
void test_promise_raceShouldCombine()
{
  volatile bool thenCalled = false;
  volatile int thenValue = 0;
  auto promiseA = std::make_shared<Promise<int>>();
  auto promiseB = std::make_shared<Promise<int>>();

  promiseA->race(promiseB)->finally(
      [&](Pair<Optional<int>, Optional<int>> pair)
      {
        thenCalled = true;
        if (pair.a.isPresent())
          thenValue = pair.a.get();
        else
          thenValue = pair.b.get();
      });

  promiseA->resolve(123);
  TEST_ASSERT_TRUE(thenCalled);
  TEST_ASSERT_EQUAL_MESSAGE(123, thenValue, "Value should from A");
  promiseB->resolve(456);

  // in this case, only the value from the first promise should be used
  TEST_ASSERT_EQUAL_MESSAGE(123, thenValue, "Value should remain from A");
}