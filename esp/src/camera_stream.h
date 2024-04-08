#pragma once
#include <Arduino.h>
#include <esp_camera.h>

#include "channel.h"
#include "globals.h"

void cameraCapture()
{
  if (!cameraOk)
    return;

  if (camSocketId == NO_SOCKET_ID)
    return;

  static unsigned long lastTime = 0;

  unsigned long interval = 1000 / cameraFps;
  unsigned long now = millis();
  if (now - lastTime < interval)
    return;

  lastTime = now;

  camera_fb_t *fb = esp_camera_fb_get();
  if (!fb)
    return;

  wsEndpoint.binary(camSocketId, fb->buf, fb->len);
  camSocketId = NO_SOCKET_ID;

  esp_camera_fb_return(fb);
}

void cameraSetFrameSize(framesize_t size)
{
  if (!cameraOk)
    return;

  sensor_t *sensor = esp_camera_sensor_get();

  sensor->set_framesize(sensor, size);
}