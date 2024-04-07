#pragma once
#include "globals.h"
#include "channel.h"
#include "data_utils.h"

void beginWebServer(Channel &chan)
{
  if (webServerActive)
    return;
  webServer.begin();
  sendData(chan, "begin", "ok");
  webServerActive = true;
}