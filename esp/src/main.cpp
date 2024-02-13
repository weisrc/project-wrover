#include <Arduino.h>
#include <ESPAsyncWebServer.h>
#include <ArduinoJson.h>

AsyncWebServer server(80);

bool scanRequested = false;
wl_status_t lastStatus;

void setup()
{
  Serial.begin(115200);

  lastStatus = WiFi.status();

  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request)
            { request->send(200, "text/plain", "hello world"); });
}

void sendReply(JsonDocument &reply)
{
  serializeJson(reply, Serial);
  Serial.println();
}

void sendData(String type, String data)
{
  JsonDocument doc;
  doc["type"] = type;
  doc["data"] = data;
  sendReply(doc);
}

void connect(JsonDocument &request)
{
  String ssid = request["ssid"];
  String auth = request["auth"];
  if (auth == "open")
  {
    WiFi.begin(ssid);
  }
  else if (auth == "wpa2")
  {
    String password = request["password"];
    WiFi.begin(ssid, password);
  }
  else if (auth == "wpa2_enterprise")
  {
    String method = request["method"];
    String identity = request["identity"];
    String username = request["username"];
    String password = request["password"];

    wpa2_auth_method_t kind = WPA2_AUTH_PEAP;
    if (method == "peap")
      kind = WPA2_AUTH_PEAP;
    else if (method == "tls")
      kind = WPA2_AUTH_TLS;
    else if (method == "ttls")
      kind = WPA2_AUTH_TTLS;
    WiFi.begin(ssid, kind, identity, username, password);
  }
}

void handleRequest(JsonDocument &request)
{
  String type = request["type"];
  if (type == "scan")
  {
    scanRequested = true;
    WiFi.scanNetworks(true);
  }
  else if (type == "connect")
    connect(request);
  else if (type == "disconnect")
    WiFi.disconnect();
  else if (type == "ip")
    sendData("ip", WiFi.localIP().toString());
  else if (type == "rssi")
    sendData("rssi", String(WiFi.RSSI()));
  else if (type == "status")
    sendData("status", String(WiFi.status()));
  else if (type == "ssid")
    sendData("ssid", WiFi.SSID());
  else if (type == "begin")
    server.begin();
}

void checkScanComplete()
{

  if (!scanRequested || WiFi.scanComplete() < 0)

    return;

  scanRequested = false;
  JsonDocument reply;
  reply["type"] = "scan";
  JsonArray networks = reply["networks"].to<JsonArray>();
  for (int i = 0; i < WiFi.scanComplete(); i++)
  {
    JsonArray network = networks.add<JsonArray>();
    network.add(WiFi.SSID(i));
    network.add(WiFi.RSSI(i));
    network.add(WiFi.encryptionType(i));
    network.add(WiFi.channel(i));
    network.add(WiFi.BSSIDstr(i));
  }
  sendReply(reply);
}

void checkStatusChange()
{
  wl_status_t status = WiFi.status();
  if (status != lastStatus)
  {
    lastStatus = status;
    sendData("status", String(status));
  }
}

void loop()
{
  if (Serial.available())
  {
    JsonDocument request;
    DeserializationError error = deserializeJson(request, Serial);
    if (error)
      return;
    handleRequest(request);
  }

  checkScanComplete();
  checkStatusChange();
}
