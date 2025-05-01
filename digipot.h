#pragma once

#include "esphome.h"

namespace esphome {
namespace digipot {

class DigiPot {
 public:
  DigiPot(uint8_t cs, uint8_t ud, uint8_t inc) : _pin_cs(cs), _pin_ud(ud), _pin_inc(inc) {
    pinMode(_pin_cs, OUTPUT);
    pinMode(_pin_ud, OUTPUT);
    pinMode(_pin_inc, OUTPUT);

    digitalWrite(_pin_cs, HIGH);
    digitalWrite(_pin_inc, HIGH);
  }

  void write(int8_t value) {
    increment(value - _digipot_value);
    _digipot_value = value;
  }

  void increment(int8_t inc) {
    bool direction = (inc >= 0);  // Direction flag
    if (inc < 0) {
      inc = -inc;  // Make inc positive
    }
    digitalWrite(_pin_cs, LOW);
    digitalWrite(_pin_ud, direction);
    for (uint8_t i = 0; i < inc; i++) {
      digitalWrite(_pin_inc, LOW);
      delayMicroseconds(5);  // Short pulse duration
      digitalWrite(_pin_inc, HIGH);
      delayMicroseconds(5);
    }
    digitalWrite(_pin_cs, HIGH);
  }

  void reset() { increment(-100); }

 private:
  uint8_t _pin_cs;
  uint8_t _pin_ud;
  uint8_t _pin_inc;
  int8_t _digipot_value = 0;
};

class DigiPotComponent : public Component {
 public:
  DigiPotComponent(uint8_t cs_pin, uint8_t ud_pin, uint8_t inc_pin)
      : digipot_(cs_pin, ud_pin, inc_pin), cs_pin_(cs_pin), ud_pin_(ud_pin), inc_pin_(inc_pin) {}

  void setup() override {
      ESP_LOGCONFIG(TAG, "Setting up DigiPot...");
      digipot_.reset();  // Reset on startup
  }

  void loop() override {}  // Nothing to do in constant loop

  void set_value(int8_t value) { digipot_.write(value); }

 private:
  DigiPot digipot_;
  uint8_t cs_pin_;
  uint8_t ud_pin_;
  uint8_t inc_pin_;

static const char *const TAG = "DigiPot";
};

}  // namespace digipot
}  // namespace esphome