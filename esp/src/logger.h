/**
 * @author Wei
 * Utility logging macros to quickly enable/disable logging
 * Implemented as macros to avoid overhead when logging is disabled
 * Hopefully the compiler will optimize out the unused branches
 */

#pragma once

#include <Arduino.h>

#define LOG_LEVEL_FATAL 0
#define LOG_LEVEL_ERROR 1
#define LOG_LEVEL_WARN 2
#define LOG_LEVEL_INFO 3
#define LOG_LEVEL_DEBUG 4

#define LOG_LEVEL LOG_LEVEL_INFO

#define LOG_FATAL(...)                \
  {                                   \
    if (LOG_LEVEL >= LOG_LEVEL_FATAL) \
    {                                 \
      Serial.print("[FATAL] ");       \
      Serial.println(__VA_ARGS__);    \
    }                                 \
  }

#define LOG_ERROR(...)                \
  {                                   \
    if (LOG_LEVEL >= LOG_LEVEL_ERROR) \
    {                                 \
      Serial.print("[ERROR] ");       \
      Serial.println(__VA_ARGS__);    \
    }                                 \
  }

#define LOG_WARN(...)                \
  {                                  \
    if (LOG_LEVEL >= LOG_LEVEL_WARN) \
    {                                \
      Serial.print("[WARN] ");       \
      Serial.println(__VA_ARGS__);   \
    }                                \
  }

#define LOG_INFO(...)                \
  {                                  \
    if (LOG_LEVEL >= LOG_LEVEL_INFO) \
    {                                \
      Serial.print("[INFO] ");       \
      Serial.println(__VA_ARGS__);   \
    }                                \
  }

#define LOG_DEBUG(...)                \
  {                                   \
    if (LOG_LEVEL >= LOG_LEVEL_DEBUG) \
    {                                 \
      Serial.print("[DEBUG] ");       \
      Serial.println(__VA_ARGS__);    \
    }                                 \
  }
