#pragma once
#include "Sensor.h"
/*
MQ-135 on A0 through a divider. ESP8266 ADC full scale is 1.0 V.
MQ-135 AO ──[ R1 220k ]──┬──► NodeMCU A0
                         │
                      [ R2 47k ]
                         │
                        GND
47k / (220k + 47k) ≈ 0.176, so 5 V >> 0.88 V at A0 (less ten 1V)
*/
class MQ135Sensor : public Sensor {
public:

  // ratio = Vpin / Vsensor, from R2 / (R1 + R2)
  explicit MQ135Sensor(float ratio = 0.176f) : _ratio(ratio) {}

  const char* name() const override { return "MQ135"; }

  bool begin() override { return true; }   // analog pin, nothing to probe

  bool read(Reading* out, uint8_t max, uint8_t& written) override {
    written = 0;
    if (max < 2) return false;

    uint32_t sum = 0;
    for (uint8_t i = 0; i < 16; i++) { sum += analogRead(A0); delay(5); }  // average
    float raw   = sum / 16.0f;             // 0..1023
    float vPin  = raw / 1023.0f;           // volts at A0
    float vSens = vPin / _ratio;           // undo divider: volts at AO

    out[written++] = {"aq_raw", raw};
    out[written++] = {"aq_v",   vSens};
    return true;
  }

private:
  float _ratio;
};