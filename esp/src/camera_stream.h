/**
 * @author Wei
 * Camera stream functions
 */

#pragma once
#include <Arduino.h>
#include <esp_camera.h>

#include "channel.h"
#include "globals.h"

void cameraCapture()
{
  if (!cameraOk)
    return;

  if (camSocketId == NO_SOCKET_ID)  // abort if no client is waiting
    return;

  static unsigned long lastTime = 0;

  unsigned long interval =
      1000 / cameraFps;  // minimum interval between frames when WiFi is exceptionally good
  unsigned long now = millis();
  if (now - lastTime < interval)
    return;

  lastTime = now;

  camera_fb_t *fb = esp_camera_fb_get();
  if (!fb)
    return;

  wsEndpoint.binary(camSocketId, fb->buf, fb->len);
  camSocketId = NO_SOCKET_ID;  // reset socket ID, if a client wants another frame, they will send
                               // another request

  esp_camera_fb_return(fb);
}

void cameraSetFrameSize(framesize_t size)
{
  if (!cameraOk)
    return;

  sensor_t *sensor = esp_camera_sensor_get();

  sensor->set_framesize(sensor, size);
}