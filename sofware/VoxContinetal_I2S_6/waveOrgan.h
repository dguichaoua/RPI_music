#ifndef WAVEORGAN_H
#define WAVEORGAN_H
#include <stdint.h>
#include <math.h>

#define MONOSYNTH_WAVE_BITS 8
#define MAXI 4095
#define HALF 2048

const uint32_t S = 4096;
const uint32_t SDIV = 65536/S;

extern int16_t tabtri16[S];
extern int16_t tabtri8[S];
extern int16_t tabtri4[S];
extern int16_t tabtriIV[S];
extern int16_t tabsin16[S];
extern int16_t tabsin8[S];
extern int16_t tabsin4[S];
extern int16_t tabsinIV[S];

float tri(uint32_t i, uint32_t L);
void genere(int waveform, int16_t wave[], uint32_t L);
void initWaves(uint32_t L);

#endif