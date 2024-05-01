/**
 * @author Wei
 * Handles everything related to movement
 */

#pragma once
#include <Arduino.h>

#include "avr_serial.h"
#include "channel.h"
#include "data_utils.h"
#include "globals.h"

int8_t motor0Speed = 0;  // target speed
int8_t motor1Speed = 0;

uint16_t sonar0Average = 0;
uint16_t sonar1Average = 0;
uint16_t sonar2Average = 0;

bool hall0Changed = false;
bool hall1Changed = false;
bool motor0Reverse = false;
bool motor1Reverse = false;
String hall;  // string with LRlr for hall data

void setMotor(Channel &chan, JsonDocument &request)
{
  motor0Speed = request["m0"].as<int8_t>();
  motor1Speed = request["m1"].as<int8_t>();
}

/**
 * Interrupt when hall sensor changes
 */
void IRAM_ATTR hall0ISR()
{
  hall0Changed = true;
}

/**
 * Interrupt when hall sensor changes
 */
void IRAM_ATTR hall1ISR()
{
  hall1Changed = true;
}

/**
 * Setup hall sensors
 */
void hallSensorSetup()
{
  pinMode(HALL0, INPUT);
  pinMode(HALL1, INPUT);
  attachInterrupt(HALL0, hall0ISR, CHANGE);
  attachInterrupt(HALL1, hall1ISR, CHANGE);
  hall.reserve(HALL_SIZE);  // reserve memory in advance to prevent unnecessary reallocations
}

/**
 * Reply to locomotion command
 */
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

/**
 * Shift value towards target by 10
 * @param current current value
 * @param target target value
 */
int8_t shiftTowards(int8_t current, int8_t target)
{
  return current + constrain(target - current, -10, 10);
}

/**
 * Update sonar data
 */
void sonarUpdate()
{
  static unsigned long lastSampleTime = 0;
  unsigned long now = millis();

  if (now - lastSampleTime < 1000)  // sample every 100ms
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

/**
 * Update motor data smoothly
 */
void motorUpdate()
{
  static unsigned long lastSampleTime = 0;
  static int8_t motor0SmoothSpeed = 1;  // start with 1 to immediately set speed to target 0
  static int8_t motor1SmoothSpeed = 1;

  unsigned long now = millis();

  if (now - lastSampleTime < 50)  // update every 50ms
    return;

  lastSampleTime = now;

  if (motor0SmoothSpeed != motor0Speed)  // send only if speed has changed
  {
    motor0SmoothSpeed = shiftTowards(motor0SmoothSpeed, motor0Speed);
    motor0Reverse = motor0SmoothSpeed < 0;
    avrSend(MODE_MOTOR0, motor0SmoothSpeed);
  }

  if (motor1SmoothSpeed != motor1Speed)
  {
    motor1SmoothSpeed = shiftTowards(motor1SmoothSpeed, motor1Speed);
    motor1Reverse = motor1SmoothSpeed < 0;
    avrSend(MODE_MOTOR1, motor1SmoothSpeed);
  }
}

/**
 * Update hall data string
 */
void locomotionUpdate()
{
  static unsigned long lastSampleTime = 0;
  unsigned long now = millis();

  if (now - lastSampleTime < 20)  // update every 20ms
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
