#pragma once
#include <Arduino.h>
#include <esp_camera.h>
#include "globals.h"
#include "channel.h"

void broadcastCapture()
{
    camera_fb_t *fb = esp_camera_fb_get();
    if (!fb)
        return;

    camEndpoint.binaryAll(fb->buf, fb->len);
}

void cameraStream()
{
    static unsigned long lastTime = 0;

    if (camEndpoint.count() == 0)
        return;

    unsigned long interval = 1000 / cameraFps;
    unsigned long now = millis();
    if (now - lastTime < interval)
        return;

    lastTime = now;

    broadcastCapture();
}

