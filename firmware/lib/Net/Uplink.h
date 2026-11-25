#pragma once
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <WiFiClient.h>
#include <ArduinoJson.h>
#include <time.h>
#include "../Sensors/Sensor.h"

class Uplink {
public:
  Uplink(const char* ssid, const char* pass, const char* url, const char* key)
    : _ssid(ssid), _pass(pass), _url(url), _key(key) {}

  // Node ID from the last 3 bytes of the MAC, e.g. "weather-a1b2c3"
  String nodeId() const {
    String mac = WiFi.macAddress();
    mac.replace(":", "");
    mac.toLowerCase();
    return "weather-" + mac.substring(6);
  }

  bool connected() const { return WiFi.status() == WL_CONNECTED; }

  bool connect(uint32_t timeoutMs = 20000) {
    WiFi.mode(WIFI_STA);
    WiFi.begin(_ssid, _pass);
    uint32_t until = millis() + timeoutMs;
    while (!connected() && millis() < until) { delay(250); Serial.print('.'); }
    Serial.println();
    if (connected())
      Serial.printf("WiFi OK, IP %s, RSSI %d dBm\n", WiFi.localIP().toString().c_str(), WiFi.RSSI());
    else
      Serial.println("WiFi FAILED");
    return connected();
  }

  // Sync clock via NTP; 1700000000 is a sanity floor (Nov 2023)
  bool syncTime(uint32_t timeoutMs = 15000) {
    configTime(0, 0, "pool.ntp.org", "time.nist.gov");
    uint32_t until = millis() + timeoutMs;
    while (time(nullptr) < 1700000000 && millis() < until) delay(250);
    return time(nullptr) >= 1700000000;
  }

  // POST one reading set. Returns true only on HTTP 201.
  bool send(const Reading* r, uint8_t count) {
    if (!connected()) return false;

    JsonDocument doc;
    doc["v"] = 1;
    doc["node"] = nodeId();
    doc["ts"] = (uint32_t)time(nullptr);
    JsonObject s = doc["sensors"].to<JsonObject>();
    for (uint8_t i = 0; i < count; i++) s[r[i].key] = r[i].value;

    String body;
    serializeJson(doc, body);

    WiFiClient client;
    HTTPClient http;
    http.begin(client, _url);
    http.addHeader("Content-Type", "application/json");
    http.addHeader("X-Node-Key", _key);
    http.setTimeout(5000);
    int code = http.POST(body);
    http.end();

    Serial.printf("POST %d  %s\n", code, body.c_str());
    return code == 201;
  }

private:
  const char* _ssid;
  const char* _pass;
  const char* _url;
  const char* _key;
};