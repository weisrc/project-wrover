/**
 * @author Wei
 * Web server begin function (command)
 */

#pragma once
#include "channel.h"
#include "data_utils.h"
#include "globals.h"

/**
 * Begin the web server
 * @param chan the channel to send the response
 */
void beginWebServer(Channel &chan)
{
  if (webServerActive)
    return;
  webServer.begin();
  sendData(chan, "begin", "ok");
  webServerActive = true;
}