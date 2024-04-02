#include <iostream>
#include <functional>
#include <memory>
#include <list>

struct none_t
{
} none;

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
        {
            listener(value);
        }
    }

    ~Promise()
    {
        if (!resolved)
        {
        }
    }
};
