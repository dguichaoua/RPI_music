#include "voice.h"

VOICE::VOICE(uint8_t key, uint8_t Vel, uint32_t inFe) {
  DivFe = 32768 / inFe;
  note = key & 0x7F;
  PhaseAcc = 0;
  OldPhaseAcc = 0;
  update(0);
}

void VOICE::next() {
  if (voiceOn) {
    PhaseAcc += PhaseInc;
    if (note == 0) {
      if (OldPhaseAcc > PhaseAcc) voiceOn = false;
    }
    OldPhaseAcc = PhaseAcc;
    uint16_t PhaseAcc1 = PhaseAcc / SDIV; 
    wavetri16 = tabtri16[PhaseAcc1];
    wavetri8 = tabtri8[PhaseAcc1];
    wavetri4 = tabtri4[PhaseAcc1];
    wavetriIV = tabtriIV[PhaseAcc1];
    wavesin16 = tabsin16[PhaseAcc1];
    wavesin8 = tabsin8[PhaseAcc1];
    wavesin4 = tabsin4[PhaseAcc1];
    wavesinIV = tabsinIV[PhaseAcc1];
  } else {
    wavetri16 = 0;
    wavetri8 = 0;
    wavetri4 = 0;
    wavetriIV = 0;
    wavesin16 = 0;
    wavesin8 = 0;
    wavesin4 = 0;
    wavesinIV = 0;
  }
}

void VOICE::update(int16_t FM) {
  PhaseInc = (tabMidi[note] >> 1) + FM;
}

void VOICE::noteOn(uint8_t key) {
  note = key & 0x7F;
  voiceOn = true;
  update(0);
}

void VOICE::noteOFF() {
  note = 0;
}
