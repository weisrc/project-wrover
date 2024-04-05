#pragma once
#include <mutex>
#include <list>
#include <Arduino.h>

#include "channel.h"

struct Message
{
    std::shared_ptr<Channel> channel;
    String data;
};

class MessageQueue
{
private:
    std::mutex mutex;
    std::list<Message> queue;

public:
    void add(std::shared_ptr<Channel> channel, String data)
    {
        std::lock_guard<std::mutex> lock(mutex);
        queue.push_back({channel, data});
    }

    Message get()
    {
        std::lock_guard<std::mutex> lock(mutex);
        Message message = queue.front();
        queue.pop_front();
        return message;
    }

    bool empty()
    {
        std::lock_guard<std::mutex> lock(mutex);
        return queue.empty();
    }
};
