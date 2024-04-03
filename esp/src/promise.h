#include <iostream>
#include <functional>
#include <memory>
#include <list>

struct none_t
{
} none;

template <typename T, typename U>
struct Pair
{
    T a;
    U b;
};

template <typename V, typename E>
class Result
{
public:
    V value;
    E error;
    bool ok;

    Result() : ok(false) {}

    Result(V value)
    {
        this->value = value;
        this->ok = true;
    }

    static Result<V, E> err(E error)
    {
        Result<V, E> result;
        result.ok = false;
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

        add([cb, promise](T value)
            {
            U result = cb(value);
            promise->resolve(result); });

        return promise;
    }

    void finally(std::function<void(T)> cb)
    {
        add([cb](T value)
            { cb(value); });
    }

    template <typename U>
    std::shared_ptr<Promise<U>> then(std::function<std::shared_ptr<Promise<U>>(T)> cb)
    {
        auto promise = std::make_shared<Promise<U>>();

        auto closure = [cb, promise](T value)
        {
            std::shared_ptr<Promise<U>> result = cb(value);

            auto closure2 = [promise](U value)
            {
                promise->resolve(value);
            };

            result->finally(closure2);
        };

        add(closure);

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
            LOG_WARN("Promise destroyed without being resolved");
        }
    }
};

template <typename U, typename V>
std::shared_ptr<Promise<Pair<U, V>>> bothPromise(std::shared_ptr<Promise<U>> promiseA, std::shared_ptr<Promise<U>> promiseB)
{
    auto promise = std::make_shared<Promise<Pair<U, V>>>();

    std::shared_ptr<Pair<U, V>> pair = std::make_shared<Pair<U, V>>();

    auto aClosure = [promiseB, pair, promise](U aValue)
    {
        pair->a = aValue;
        LOG_DEBUG("aClosure ran");
        if (promiseB->isResolved())
        {
            promise->resolve(*pair);
            LOG_DEBUG("resolving both from A");
        }
    };

    auto bClosure = [promiseA, pair, promise](U bValue)
    {
        pair->b = bValue;
        LOG_DEBUG("bClosure ran");
        if (promiseA->isResolved())
        {
            promise->resolve(*pair);
            LOG_DEBUG("resolving both from B");
        }
    };

    promiseA->finally(aClosure);
    promiseB->finally(bClosure);

    return promise;
}