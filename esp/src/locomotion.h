#pragma once
#include <Arduino.h>

#include "avr_serial.h"
#include "channel.h"
#include "data_utils.h"
#include "globals.h"

int8_t motor0Speed = 0;
int8_t motor1Speed = 0;

#define SONAR_ARRAY_SIZE 5

uint16_t sonar0Average = 0;
uint16_t sonar1Average = 0;
uint16_t sonar2Average = 0;

bool hall0Changed = false;
bool hall1Changed = false;
bool motor0Reverse = false;
bool motor1Reverse = false;
String hall;

void setMotor(Channel &chan, JsonDocument &request)
{
  motor0Speed = request["m0"].as<int8_t>();
  motor1Speed = request["m1"].as<int8_t>();
}

void IRAM_ATTR hall0ISR()
{
  hall0Changed = true;
}

void IRAM_ATTR hall1ISR()
{
  hall1Changed = true;
}

void hallSensorSetup()
{
  pinMode(HALL0, INPUT);
  pinMode(HALL1, INPUT);
  attachInterrupt(HALL0, hall0ISR, CHANGE);
  attachInterrupt(HALL1, hall1ISR, CHANGE);
  hall.reserve(HALL_SIZE);
}

void locomotionReply(Channel &chan)
{
  JsonDocument data;
  data["type"] = "locomotion";
  data["hall"] = hall;
  JsonArray sonars = data["sonar"].to<JsonArray>();
  sonars.add(sonar0Average);
  sonars.add(sonar1Average);
  sonars.add(sonar2Average);
  chan.send(data);
  hall.clear();
}

int8_t shiftTowards(int8_t current, int8_t target)
{
  if (current < target)
    return current + 1;
  else if (current > target)
    return current - 1;
  return current;
}

void sonarUpdate()
{
  static unsigned long lastSampleTime = 0;
  unsigned long now = millis();

  if (now - lastSampleTime < 100)
    return;

  lastSampleTime = now;

  auto closure0 = [](WordResult result)
  {
    if (result.isOk())
      sonar0Average = result.getValue();
  };

  auto closure1 = [](WordResult result)
  {
    if (result.isOk())
      sonar1Average = result.getValue();
  };

  auto closure2 = [](WordResult result)
  {
    if (result.isOk())
      sonar2Average = result.getValue();
  };

  avrSonar(MODE_SONAR0)->finally(closure0);
  avrSonar(MODE_SONAR1)->finally(closure1);
  avrSonar(MODE_SONAR2)->finally(closure2);
}

void motorUpdate()
{
  static unsigned long lastSampleTime = 0;
  static int8_t motor0SmoothSpeed = 0;
  static int8_t motor1SmoothSpeed = 0;

  unsigned long now = millis();

  if (now - lastSampleTime < 50)
    return;

  if (motor0SmoothSpeed != motor0Speed)
  {
    int8_t motor0NextSpeed = shiftTowards(motor0SmoothSpeed, motor0Speed);

    auto closure = [motor0NextSpeed](WriteResult result)
    {
      if (result.isOk())
      {
        motor0SmoothSpeed = motor0NextSpeed;
        motor0Reverse = motor0SmoothSpeed < 0;
      }
    };

    avrSend(MODE_MOTOR0, motor0NextSpeed)->finally(closure);
  }

  if (motor1SmoothSpeed != motor1Speed)
  {
    int8_t motor1NextSpeed = shiftTowards(motor1SmoothSpeed, motor1Speed);

    auto closure = [motor1NextSpeed](WriteResult result)
    {
      if (result.isOk())
      {
        motor1SmoothSpeed = motor1NextSpeed;
        motor1Reverse = motor1SmoothSpeed < 0;
      }
    };

    avrSend(MODE_MOTOR1, motor1NextSpeed)->finally(closure);
  }
}

void locomotionUpdate()
{
  static unsigned long lastSampleTime = 0;
  unsigned long now = millis();

  if (now - lastSampleTime < 20)
    return;

  lastSampleTime = now;

  if (hall.length() >= HALL_SIZE)
    hall.clear();

  if (hall0Changed)
  {
    odometer.moveLeft(motor0Reverse);
    hall += motor0Reverse ? 'L' : 'l';
  }

  if (hall1Changed)
  {
    odometer.moveRight(motor1Reverse);
    hall += motor1Reverse ? 'R' : 'r';
  }

  hall0Changed = false;
  hall1Changed = false;
}
