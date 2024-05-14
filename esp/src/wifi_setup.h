/**
 * @author Wei
 * WiFi setup
 */

#pragma once
#include <Arduino.h>
#include "WiFi.h"

#include "logger.h"

void handleWiFiEvent(WiFiEvent_t event) {
  LOG_INFO("[WiFi-event] event: " + event);
  switch (event) {
    case SYSTEM_EVENT_WIFI_READY:
      LOG_INFO("WiFi interface ready");
      break;
    case SYSTEM_EVENT_SCAN_DONE:
      LOG_INFO("Completed scan for access points");
      break;
    case SYSTEM_EVENT_STA_START:
      LOG_INFO("WiFi client started");
      break;
    case SYSTEM_EVENT_STA_STOP:
      LOG_INFO("WiFi clients stopped");
      break;
    case SYSTEM_EVENT_STA_CONNECTED:
      LOG_INFO("Connected to access point");
      break;
    case SYSTEM_EVENT_STA_DISCONNECTED:
      LOG_INFO("Disconnected from WiFi access point");
      break;
    case SYSTEM_EVENT_STA_AUTHMODE_CHANGE:
      LOG_INFO("Authentication mode of access point has changed");
      break;
    case SYSTEM_EVENT_STA_GOT_IP:
      LOG_INFO("Obtained IP address: " + WiFi.localIP());
      break;
    case SYSTEM_EVENT_STA_LOST_IP:
      LOG_INFO("Lost IP address and IP address is reset to 0");
      break;
    case SYSTEM_EVENT_STA_WPS_ER_SUCCESS:
      LOG_INFO("WiFi Protected Setup (WPS): succeeded in enrollee mode");
      break;
    case SYSTEM_EVENT_STA_WPS_ER_FAILED:
      LOG_INFO("WiFi Protected Setup (WPS): failed in enrollee mode");
      break;
    case SYSTEM_EVENT_STA_WPS_ER_TIMEOUT:
      LOG_INFO("WiFi Protected Setup (WPS): timeout in enrollee mode");
      break;
    case SYSTEM_EVENT_STA_WPS_ER_PIN:
      LOG_INFO("WiFi Protected Setup (WPS): pin code in enrollee mode");
      break;
    case SYSTEM_EVENT_AP_START:
      LOG_INFO("WiFi access point started");
      break;
    case SYSTEM_EVENT_AP_STOP:
      LOG_INFO("WiFi access point  stopped");
      break;
    case SYSTEM_EVENT_AP_STACONNECTED:
      LOG_INFO("Client connected");
      break;
    case SYSTEM_EVENT_AP_STADISCONNECTED:
      LOG_INFO("WiFi client disconnected");
      break;
    case SYSTEM_EVENT_AP_STAIPASSIGNED:
      LOG_INFO("Assigned IP address to client");
      break;
    case SYSTEM_EVENT_AP_PROBEREQRECVED:
      LOG_INFO("Received probe request");
      break;
    case SYSTEM_EVENT_GOT_IP6:
      LOG_INFO("IPv6 is preferred");
      break;
    case SYSTEM_EVENT_ETH_GOT_IP:
      LOG_INFO("Obtained IP address");
      break;
  }
}

void wifiSetup() {
  // WiFi.onEvent(handleWiFiEvent);
  // Can't use the above since got Guru meditation error Load Prohibited
  WiFi.mode(WIFI_STA);
}