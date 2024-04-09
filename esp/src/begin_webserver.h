#pragma once
#include "channel.h"
#include "data_utils.h"
#include "globals.h"

void beginWebServer(Channel &chan)
{
  if (webServerActive)
    return;
  webServer.begin();
  sendData(chan, "begin", "ok");
  webServerActive = true;
}