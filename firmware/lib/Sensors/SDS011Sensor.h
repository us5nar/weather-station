#pragma once
#include <SoftwareSerial.h>
#include "Sensor.h"

class SDS011Sensor : public Sensor {
public:
  SDS011Sensor(uint8_t rxPin, uint8_t txPin) : _serial(rxPin, txPin) {}

  const char* name() const override { return "SDS011"; }

  bool begin() override {
    _serial.begin(9600);
    return true;                 // no handshake; "no data" shows up in read()
  }

  bool read(Reading* out, uint8_t max, uint8_t& written) override {
    written = 0;
    if (max < 2) return false;

    uint8_t buf[10];
    bool found = false;
    float pm25 = 0, pm10 = 0;

    // Sensor streams ~1 Hz; keep the last valid frame seen in ~3 s
    uint32_t until = millis() + 3000;
    while (millis() < until) {
      if (_serial.available() < 10) { yield(); continue; }
      if (_serial.read() != 0xAA) continue;       // header
      buf[0] = 0xAA;
      _serial.readBytes(buf + 1, 9);
      if (buf[1] != 0xC0 || buf[9] != 0xAB) continue;   // command + tail
      uint8_t sum = 0;
      for (int i = 2; i <= 7; i++) sum += buf[i];       // checksum
      if (sum != buf[8]) continue;
      pm25 = ((buf[3] << 8) | buf[2]) / 10.0f;
      pm10 = ((buf[5] << 8) | buf[4]) / 10.0f;
      found = true;
    }

    if (!found) return false;
    out[written++] = {"pm25", pm25};
    out[written++] = {"pm10", pm10};
    return true;
  }

private:
  SoftwareSerial _serial;
};