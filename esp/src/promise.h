#pragma once
#include <functional>
#include <iostream>
#include <list>
#include <memory>

template <typename T, typename U>
struct Pair
{
  T a;
  U b;
};

template <typename T>
class Optional
{
private:
  T value;
  bool hasValue;

public:
  Optional() : hasValue(false) {}

  Optional(T value) : value(value), hasValue(true) {}

  T get()
  {
    return value;
  }

  bool isEmpty()
  {
    return hasValue;
  }

  bool isPresent()
  {
    return hasValue;
  }

  static Optional<T> empty()
  {
    return Optional<T>();
  }
};

template <typename V, typename E>
class Result
{
private:
  V value;
  E error;
  bool hasValue;

public:
  Result() : hasValue(false) {}

  Result(V value) : value(value), hasValue(true) {}

  bool isOk()
  {
    return hasValue;
  }

  bool isError()
  {
    return !hasValue;
  }

  V getValue()
  {
    return value;
  }

  E getError()
  {
    return error;
  }

  static Result<V, E> fail(E error)
  {
    Result<V, E> result;
    result.error = error;
    return result;
  }
};

template <typename T>
class Promise
{
private:
  bool resolved = false;
  T value;
  std::list<std::function<void(T)>> listeners;

  void add(std::function<void(T)> listener)
  {
    if (resolved)
      listener(value);
    else
      listeners.push_back(listener);
  }

public:
  template <typename U>
  std::shared_ptr<Promise<U>> then(std::function<U(T)> cb)
  {
    auto promise = std::make_shared<Promise<U>>();

    add(
        [cb, promise](T value)
        {
          U result = cb(value);
          promise->resolve(result);
        });

    return promise;
  }

  void finally(std::function<void(T)> cb)
  {
    add([cb](T value) { cb(value); });
  }

  template <typename U>
  std::shared_ptr<Promise<U>> then(std::function<std::shared_ptr<Promise<U>>(T)> cb)
  {
    auto promise = std::make_shared<Promise<U>>();

    auto closure = [cb, promise](T value)
    {
      std::shared_ptr<Promise<U>> result = cb(value);

      auto closure2 = [promise](U value) { promise->resolve(value); };

      result->finally(closure2);
    };

    add(closure);

    return promise;
  }

  template <typename U>
  std::shared_ptr<Promise<Pair<T, U>>> pair(std::shared_ptr<Promise<U>> other)
  {
    auto promise = std::make_shared<Promise<Pair<T, U>>>();
    auto pair = std::make_shared<Pair<T, U>>();

    auto thisClosure = [other, pair, promise](U aValue)
    {
      pair->a = aValue;
      // LOG_DEBUG("thisClosure ran");
      if (other->isResolved())
      {
        promise->resolve(*pair);
        // LOG_DEBUG("resolving both from A");
      }
    };

    auto otherClosure = [&, pair, promise](U bValue)
    {
      pair->b = bValue;
      // LOG_DEBUG("otherClosure ran");
      if (isResolved())
      {
        promise->resolve(*pair);
        // LOG_DEBUG("resolving both from B");
      }
    };

    finally(thisClosure);
    other->finally(otherClosure);

    return promise;
  }

  template <typename U>
  std::shared_ptr<Promise<Pair<Optional<T>, Optional<U>>>> race(std::shared_ptr<Promise<U>> other)
  {
    auto promise = std::make_shared<Promise<Pair<Optional<T>, Optional<U>>>>();
    auto pair = std::make_shared<Pair<Optional<T>, Optional<U>>>();

    auto thisClosure = [other, pair, promise](T aValue)
    {
      pair->a = Optional<T>(aValue);
      // LOG_DEBUG("thisClosure ran");
      promise->resolve(*pair);
    };

    auto otherClosure = [this, pair, promise](U bValue)
    {
      pair->b = Optional<U>(bValue);
      // LOG_DEBUG("otherClosure ran");
      promise->resolve(*pair);
    };

    finally(thisClosure);
    other->finally(otherClosure);

    return promise;
  }

  void resolve(T value)
  {
    if (resolved)
      return;

    resolved = true;
    this->value = value;
    for (auto listener : listeners)
      listener(value);

    listeners.clear();
  }

  bool isResolved()
  {
    return resolved;
  }

  ~Promise()
  {
    if (!resolved)
    {
      // LOG_WARN("Promise destroyed without being resolved");
    }
  }
};
