#include <Arduino.h>
#include <esp_camera.h>
#include "globals.h"

void cameraStream()
{
    if (cameraSocketId == NO_SOCKET_ID)
        return;

    AsyncWebSocketClient *client = ws.client(cameraSocketId);

    if (!client)
        return;

    unsigned long interval = 1000 / cameraFps;
    unsigned long now = millis();
    if (now - cameraLastTime < interval)
        return;

    cameraLastTime = now;

    camera_fb_t *fb = esp_camera_fb_get();
    if (!fb)
        return;

    client->binary((uint8_t *)fb->buf, fb->len);

    esp_camera_fb_return(fb);
}