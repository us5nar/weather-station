#include <Arduino.h>
//#include ".\src\lib\Sensors\BME280Sensor.h"
#include "C:\Users\US5NAR\Documents\PlatformIO\Projects\weather-station\firmware\lib\Sensors\BME280Sensor.h"
// #include "SDS011Sensor.h"
#include "C:\Users\US5NAR\Documents\PlatformIO\Projects\weather-station\firmware\lib\Sensors\SDS011Sensor.h"

constexpr uint32_t INTERVAL_MS  = 60000;
constexpr uint8_t  MAX_READINGS = 8;

BME280Sensor bme(0x76);            // try 0x77 if not found
SDS011Sensor sds(D5, D6);          // SDS011 TX -> D5, RX <- D6

// Enable/disable later: build this list from config.json
Sensor* sensors[] = { &bme, &sds };
constexpr uint8_t SENSOR_COUNT = sizeof(sensors) / sizeof(sensors[0]);

Reading readings[MAX_READINGS];
uint32_t lastRead;

void setup() {
  Serial.begin(115200);
  for (uint8_t i = 0; i < SENSOR_COUNT; i++) {
    bool ok = sensors[i]->begin();
    Serial.printf("%s: %s\n", sensors[i]->name(), ok ? "OK" : "NOT FOUND");
  }
  lastRead = millis() - INTERVAL_MS;   // read immediately on boot
}

void loop() {
  if (millis() - lastRead < INTERVAL_MS) { yield(); return; }
  lastRead = millis();

  uint8_t count = 0;
  for (uint8_t i = 0; i < SENSOR_COUNT; i++) {
    uint8_t written = 0;
    if (!sensors[i]->read(readings + count, MAX_READINGS - count, written)) {
      Serial.printf("[%s no data] ", sensors[i]->name());
    }
    count += written;
  }

  // key=value line, keys match the server JSON
  for (uint8_t i = 0; i < count; i++) {
    Serial.print(readings[i].key);
    Serial.print('=');
    Serial.print(readings[i].value, 1);
    Serial.print(' ');
  }
  Serial.println();
}