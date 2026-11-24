#pragma once
#include <Adafruit_BME280.h>
#include "Sensor.h"

class BME280Sensor : public Sensor {
public:
  explicit BME280Sensor(uint8_t addr) : _addr(addr) {}

  const char* name() const override { return "BME280"; }

  bool begin() override {
    _ok = _bme.begin(_addr);
    return _ok;
  }

  bool read(Reading* out, uint8_t max, uint8_t& written) override {
    written = 0;
    if (!_ok || max < 3) return false;
    out[written++] = {"temp_c",    _bme.readTemperature()};
    out[written++] = {"hum_pct",   _bme.readHumidity()};
    out[written++] = {"press_hpa", _bme.readPressure() / 100.0f};  // Pa -> hPa
    return true;
  }

private:
  Adafruit_BME280 _bme;
  uint8_t _addr;
  bool _ok = false;
};