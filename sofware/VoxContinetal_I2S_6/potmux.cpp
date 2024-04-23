#include "potmux.h"

POTMUX::POTMUX(uint8_t MX1, uint8_t MX2, uint8_t MX3, uint8_t AN) {
  DMUX1 = MX1;
  DMUX2 = MX2;
  DMUX3 = MX3;
  ADC_IN = AN;
  //Mux
  pinMode(DMUX1, OUTPUT);
  pinMode(DMUX2, OUTPUT);
  pinMode(DMUX3, OUTPUT);
  muxval = 0;
  // Select ADC input 0 (GPIO26) & 1
  adc_init();
  // Make sure GPIO is high-impedance, no pullups etc
  adc_gpio_init(ADC_IN + 26);
  adc_select_input(ADC_IN);
  for (uint8_t i = 0; i < 8; i++) {
    mux(i);
    pot[i] = potentiometers();
    oldpot[i] = pot[i];
  }
}

void POTMUX::mux(uint8_t number) {
  switch (number) {
    case 0:
      gpio_put(DMUX1, false);
      gpio_put(DMUX2, false);
      gpio_put(DMUX3, false);
      break;
    case 4:
      gpio_put(DMUX1, true);
      gpio_put(DMUX2, false);
      gpio_put(DMUX3, false);
      break;
    case 1:
      gpio_put(DMUX1, false);
      gpio_put(DMUX2, true);
      gpio_put(DMUX3, false);
      break;
    case 5:
      gpio_put(DMUX1, true);
      gpio_put(DMUX2, true);
      gpio_put(DMUX3, false);
      break;
    case 2:
      gpio_put(DMUX1, false);
      gpio_put(DMUX2, false);
      gpio_put(DMUX3, true);
      break;
    case 6:
      gpio_put(DMUX1, true);
      gpio_put(DMUX2, false);
      gpio_put(DMUX3, true);
      break;
    case 3:
      gpio_put(DMUX1, false);
      gpio_put(DMUX2, true);
      gpio_put(DMUX3, true);
      break;
    case 7:
      gpio_put(DMUX1, true);
      gpio_put(DMUX2, true);
      gpio_put(DMUX3, true);
      break;
  }
  // allow 50 us for signals to stablize
  delayMicroseconds(100);
}

uint16_t POTMUX::potentiometers() {
  adc_select_input(ADC_IN);
  adcval = 0;
  for (uint8_t i; i < mean; i++) adcval += adc_read();
  return (adcval / mean) >> 5;
}

int8_t POTMUX::update() {
  change = -1;
  muxval++;
  if (muxval >= 8) muxval = 0;
  mux(muxval);
  pot[muxval] = potentiometers();
  if (oldpot[muxval] != pot[muxval]) {
    oldpot[muxval] = pot[muxval];
    change = muxval;
  }
  return change;
}

uint16_t POTMUX::get() {
  if (change < 0) return 0;
  return pot[change];
}