#include <Arduino.h>
#include <esp_camera.h>
#include "globals.h"

void cameraStream()
{
    static unsigned long lastTime = 0;

    if (cameraSocketId == NO_SOCKET_ID)
        return;

    AsyncWebSocketClient *client = wsEndpoint.client(cameraSocketId);

    if (!client)
        return;

    unsigned long interval = 1000 / cameraFps;
    unsigned long now = millis();
    if (now - lastTime < interval)
        return;

    lastTime = now;

    camera_fb_t *fb = esp_camera_fb_get();
    if (!fb)
        return;

    client->binary((uint8_t *)fb->buf, fb->len);

    esp_camera_fb_return(fb);
}