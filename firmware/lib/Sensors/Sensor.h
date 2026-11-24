#pragma once
#include <Arduino.h>

struct Reading {
  const char* key;   // matches the server JSON key, e.g. "pm25"
  float value;
};

class Sensor {
public:
  virtual ~Sensor() {}
  virtual const char* name() const = 0;     // "BME280"
  virtual bool begin() = 0;                 // false = not found
  // Fills up to `max` readings, sets `written`. Returns false if no valid data.
  virtual bool read(Reading* out, uint8_t max, uint8_t& written) = 0;
};