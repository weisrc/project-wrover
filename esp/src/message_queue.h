/**
 * @author Wei
 * Message queue for storing messages
 * This is thread-safe
 */

#pragma once

#include <Arduino.h>

#include <list>
#include <mutex>

#include "channel.h"

struct Message
{
  std::shared_ptr<Channel> channel;
  String data;
};

/**
 * Message queue for storing messages
 */
class MessageQueue
{
private:
  std::mutex mutex;
  std::list<Message> queue;  // List used as a queue as it is O(1) for push_back and pop_front

public:
  void add(std::shared_ptr<Channel> channel, String data)
  {
    std::lock_guard<std::mutex> lock(mutex);
    queue.push_back({channel, data});
  }

  Message get()
  {
    std::lock_guard<std::mutex> lock(mutex);  // Lock the mutex
    Message message = queue.front();
    queue.pop_front();
    return message;
    // Mutex destructor is called here as it goes out of scope
  }

  bool empty()
  {
    std::lock_guard<std::mutex> lock(mutex);
    return queue.empty();
  }
};
