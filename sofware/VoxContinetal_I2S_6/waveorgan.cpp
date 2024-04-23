#include "waveOrgan.h"

int16_t tabtri16[S];
int16_t tabtri8[S];
int16_t tabtri4[S];
int16_t tabtriIV[S];
int16_t tabsin16[S];
int16_t tabsin8[S];
int16_t tabsin4[S];
int16_t tabsinIV[S];

float tri(uint32_t i, uint32_t L) {
  uint32_t HS1 = L / 2;
  uint32_t QS1 = L / 4;
  uint32_t QS31 = 3 * L / 4;
  float iQS1 = 1.0 / QS1;
  float w;

  i %= L;

  if (i < QS1) w = i * iQS1;
  else if (i < HS1) w = (1.0 - (i - QS1) * iQS1);
  else if (i < QS31) w = 0.0 - (i - HS1) * iQS1;
  else w = (-1.0 + (i - QS31) * iQS1);

  return w;
}

void genere(int waveform, int16_t wave[], uint32_t L) {

  uint32_t HS = L / 2;
  uint32_t QS = L / 4;
  uint32_t QS3 = QS * 3;
  float iQS = 1.0 / QS;

  const float a5_13 = 1.49830707687668;
  const float a2_23 = a5_13 * 2;
  const float a1_35 = 5.03968419957949;

  uint32_t S16 = L;
  uint32_t S8 = L / 2;
  uint32_t S4 = L / 4;
  uint32_t S2 = L / 8;
  uint32_t S1 = L / 16;
  uint32_t S5_13 = L / a5_13;
  uint32_t S2_23 = L / a2_23;
  uint32_t S1_35 = L / a1_35;

  switch (waveform) {
    case 0:
      for (uint32_t i = 0; i < L; i++) wave[i] = MAXI * tri(i, S16);
      break;
    case 1:
      for (uint32_t i = 0; i < L; i++) wave[i] = MAXI * tri(i, S8);
      break;
    case 2:
      for (uint32_t i = 0; i < L; i++) wave[i] = MAXI * tri(i, S4);
      break;
    case 3:
      for (uint32_t i = 0; i < L; i++) wave[i] = (int32_t)(MAXI * (tri(i, S5_13) + tri(i, S2) + tri(i, S2_23) + tri(i, S1_35) + tri(i, S1))) >> 2;
      break;
    case 4:
      for (uint32_t i = 0; i < L; i++)
        wave[i] = MAXI * sin(2 * i * M_PI / S16);
      break;
    case 5:
      for (uint32_t i = 0; i < L; i++)
        wave[i] = MAXI * sin(2 * i * M_PI / S8);
      break;
    case 6:
      for (uint32_t i = 0; i < L; i++)
        wave[i] = MAXI * sin(2 * i * M_PI / S4);
      break;
    case 7:
      for (uint32_t i = 0; i < L; i++)
        wave[i] = (int32_t)(MAXI * (sin(a5_13 * 2 * i * M_PI / L) + sin(8 * i * M_PI / L) + sin(a2_23 * 2 * i * M_PI / L) + sin(16 * i * M_PI / L) + sin(a1_35 * 2 * i * M_PI / L))) >> 2;
      break;
  }
}

void initWaves(uint32_t L) {
  genere(0, tabtri16, L);
  genere(1, tabtri8, L);
  genere(2, tabtri4, L);
  genere(3, tabtriIV, L);
  genere(4, tabsin16, L);
  genere(5, tabsin8, L);
  genere(6, tabsin4, L);
  genere(7, tabsinIV, L);
}
