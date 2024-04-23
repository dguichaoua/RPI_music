#ifndef VOICE_H
#define VOICE_H
#include <stdint.h>
#include <math.h>
#include "waveOrgan.h"
#include "tableMidi.h"
#include <cstdlib>  // Pour utiliser la fonction rand()

class VOICE {
public:
  VOICE() = default;
  VOICE(uint8_t key, uint8_t Vel, uint32_t inFe);
  void next();
  void update(int16_t FM);
  void noteOn(uint8_t key);
  void noteOFF();
  uint8_t note = 0;
  uint32_t DivFe = 1;
  int16_t wavetri8;
  int16_t wavetri16;
  int16_t wavetri4;
  int16_t wavetriIV;
  int16_t wavesin8;
  int16_t wavesin16;
  int16_t wavesin4;
  int16_t wavesinIV;


private:
  uint16_t PhaseAcc;
  uint16_t PhaseInc;
  bool voiceOn = false;
  uint16_t OldPhaseAcc;
};

#endif