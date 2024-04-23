#ifndef POTMUX_H
#define POTMUX_H

#include <stdint.h>
#include <Arduino.h>
#include <hardware/gpio.h>
#include "hardware/adc.h"

class POTMUX {
public:
  POTMUX(uint8_t MX1, uint8_t MX2, uint8_t MX3, uint8_t AN);
  void mux(uint8_t number);
  uint16_t potentiometers();
  int8_t update();
  uint16_t get();

private:
  uint8_t DMUX1;
  uint8_t DMUX2;
  uint8_t DMUX3;
  uint8_t ADC_IN;
  uint32_t adcval;
  uint16_t pot[8];
  uint16_t oldpot[8];
  const uint8_t mean = 128;
  uint8_t muxval;
  int8_t change;
};

#endif