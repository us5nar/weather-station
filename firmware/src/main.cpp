#include <Arduino.h>
#include <secrets.h>
#include <Sensors/BME280Sensor.h>
#include <Sensors/SDS011Sensor.h>
#include <Sensors/MQ135Sensor.h>
#include <Net/Uplink.h>

constexpr uint32_t INTERVAL_MS  = 60000;
constexpr uint8_t  MAX_READINGS = 8;      // bme 3 + sds 2 + mq135 2

BME280Sensor bme(0x76);            // try 0x77 if not found
SDS011Sensor sds(D5, D6);          // SDS011 TX -> D5, RX <- D6
MQ135Sensor  mq;                   // A0, divider ratio 0.176
Sensor* sensors[] = { &bme, &sds, &mq };
constexpr uint8_t SENSOR_COUNT = sizeof(sensors) / sizeof(sensors[0]);

Uplink uplink(WIFI_SSID, WIFI_PASS, SERVER_URL, NODE_KEY);
Reading readings[MAX_READINGS];
uint32_t lastRead;

void setup() {
  Serial.begin(115200);
  for (uint8_t i = 0; i < SENSOR_COUNT; i++) {
    bool ok = sensors[i]->begin();
    Serial.printf("%s: %s\n", sensors[i]->name(), ok ? "OK" : "NOT FOUND");
  }

  uplink.connect();
  if (uplink.connected()) {
    Serial.println(uplink.syncTime() ? "NTP OK" : "NTP FAILED");
    Serial.printf("Node ID: %s\n", uplink.nodeId().c_str());
  }
  lastRead = millis() - INTERVAL_MS;   // read immediately on boot
}

void loop() {
  if (millis() - lastRead < INTERVAL_MS) { yield(); return; }
  lastRead = millis();

  if (!uplink.connected()) uplink.connect();   // simple reconnect

  uint8_t count = 0;
  for (uint8_t i = 0; i < SENSOR_COUNT; i++) {
    uint8_t written = 0;
    if (!sensors[i]->read(readings + count, MAX_READINGS - count, written))
      Serial.printf("[%s no data] ", sensors[i]->name());
    count += written;
  }

  for (uint8_t i = 0; i < count; i++) {
    Serial.print(readings[i].key); Serial.print('=');
    Serial.print(readings[i].value, 2); Serial.print(' ');
  }
  Serial.println();

  uplink.send(readings, count);      // failed sends are only logged for now
}