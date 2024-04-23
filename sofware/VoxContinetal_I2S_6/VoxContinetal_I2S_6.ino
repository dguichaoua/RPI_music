/*

    RPI PICO        PT8211
     GPIO2(4)     BCK (1)
     GPIO5(7)     DIN (3)
     GPIO3(5)       WS(2)
     GND            (4)
     +3.3V          (5)

     Sortie audio L&R en (6) et (8)
*/

#include <I2S.h>
#include "voice.h"
#include <Adafruit_TinyUSB.h>
#include <MIDI.h>
#include <OneButton.h>
#include <elapsedMillis.h>
//#include <hardware/gpio.h>
#include "hardware/adc.h"
#include "lfo.h"
#include "pin.h"
#include "potmux.h"

#define C1 24 + 12
#define C6 84 + 12

// USB MIDI object
Adafruit_USBD_MIDI usb_midi;

// Create a new instance of the Arduino MIDI Library,
// and attach usb_midi as the transport.
MIDI_CREATE_INSTANCE(Adafruit_USBD_MIDI, usb_midi, MIDI);

// Create the I2S port using a PIO state machine
I2S i2s(OUTPUT);

//Potentiometres
POTMUX pot(MUX1, MUX2, MUX3, ADC);

bool button1State = false;
bool button2State = false;
bool button3State = false;
bool button4State = false;

//Bounce button
OneButton btn1 = OneButton(
  SW1,   // Input pin for the button
  true,  // Button is active LOW
  true   // Enable internal pull-up resistor
);
OneButton btn2 = OneButton(
  SW2,   // Input pin for the button
  true,  // Button is active LOW
  true   // Enable internal pull-up resistor
);
OneButton btn3 = OneButton(
  SW3,   // Input pin for the button
  true,  // Button is active LOW
  true   // Enable internal pull-up resistor
);
OneButton btn4 = OneButton(
  SW4,   // Input pin for the button
  true,  // Button is active LOW
  true   // Enable internal pull-up resistor
);

elapsedMillis msec = 0;

// GPIO pin numbers
#define pBCLK 2
#define pWS (pBCLK + 1)
#define pDOUT 5

#define NBPOINT 65536

#define MAXVOICE 10

const uint32_t Fe = 65536;  //Fréquence d'échantillonage
const uint8_t FeMod = 64;   //Fréquence d'échantillonage des modulations
const uint16_t razCount = Fe / FeMod;
const uint32_t delaysize = Fe * 256 / 1000;  //Delay de 256ms max

int16_t Level[] = { 16, 64, 32, 16, 16, 16, 1, 1, 1 };

VOICE *Voices = new VOICE[MAXVOICE];

int count = 0;
bool vibratoOn = false;
bool chorusOn = false;
bool delayOn = false;

int32_t Chorusline[256];
uint8_t Chorusprt = 0;
uint8_t ChorusMix = 96;
uint8_t ChorusFreq = 1;

int32_t Delayline[delaysize];
uint32_t Delayprt = 0;
uint32_t DelayTime = delaysize;
uint8_t DelayWetGain = 64;
uint8_t DelayFeedback = 32;
int32_t DelayOld;

uint8_t lfo1prt = 0;
uint8_t lfo2prt = 0;
int8_t lfoval1 = 0;
int16_t lfoval2 = 0;
uint8_t VibratoLevel = 8;
uint8_t VibratoFreq = 21;

int32_t wave = 0;
int32_t wavetri = 0;
int32_t wavesin = 0;
int32_t waveL = 0;
int32_t waveR = 0;
int32_t waveDelay = 0;
int32_t Chorus = 0;

float Amp = 0;
uint8_t Ampi = 0;
int8_t Vel = 0;
uint8_t EnvTime = 16;
int8_t LastNote = -1;
int8_t NbNote = 0;

// Handler function for a single click:
static void handleClick1() {
  if (button1State) {
    button1State = false;
  } else {
    button1State = true;
  }
  gpio_put(LED1, button1State);
  vibratoOn = button1State;
}
static void handleClick2() {
  if (button2State) {
    button2State = false;
  } else {
    button2State = true;
  }
  gpio_put(LED2, button2State);
  chorusOn = button2State;
}
static void handleClick3() {
  if (button3State) {
    button3State = false;
  } else {
    button3State = true;
  }
  gpio_put(LED3, button3State);
  delayOn = button3State;
}
static void handleClick4() {
  if (button4State) {
    button4State = false;
  } else {
    button4State = true;
  }
  gpio_put(LED4, button4State);
}

void adjustLevel() {
  Level[6] = (Level[0] + Level[1] + Level[2] + 1);
  Level[7] = (127 + Level[3]);
  Level[8] = (Level[4] + Level[5] + 1);
}

void handleNoteOn(byte channel, byte pitch, byte velocity) {
  if (pitch < C1 || pitch > C6) return;
  int noteFree = -1;
  if (velocity) {
    for (int i = 0; i < MAXVOICE; i++) {
      if (Voices[i].note == pitch) return;
    }
    for (int i = 0; i < MAXVOICE; i++) {
      if (Voices[i].note == 0) {
        noteFree = i;
        break;
      }
    }
    if (noteFree >= 0) {
      Voices[noteFree].noteOn(pitch);
      Vel = 64;
      gpio_put(LED_BUILTIN, true);
    }
  } else handleNoteOff(channel, pitch, 0);
}

void handleNoteOff(byte channel, byte pitch, byte velocity) {
  if (pitch < C1 || pitch > C6) return;
  /*for (int i = 0; i < MAXVOICE; i++) {
    if (Voices[i].note) NbNote++;
  }*/
  for (int i = 0; i < MAXVOICE; i++) {
    if (Voices[i].note == pitch) {
      /*if (NbNote == 1) {
        LastNote = i;
        Vel = 0;
      }
      else*/
      Voices[i].noteOFF();
      break;
    }
  }
  gpio_put(LED_BUILTIN, false);
}

void handleControlChange(byte channel, byte controller, byte value) {
  switch (controller) {
    case 9:
      Level[0] = value;
      adjustLevel();
      break;
    case 10:
      Level[1] = value;
      adjustLevel();
      break;
    case 11:
      Level[2] = value;
      adjustLevel();
      break;
    case 12:
      Level[3] = value;
      adjustLevel();
      break;
    case 13:
      Level[4] = value;
      adjustLevel();
      break;
    case 14:
      Level[5] = value;
      adjustLevel();
      break;
    case 15:
      if (value) {
        vibratoOn = true;
        button1State = true;
        gpio_put(LED1, button1State);
      } else {
        vibratoOn = false;
        button1State = false;
        gpio_put(LED1, button1State);
      }
      break;
    case 16:
      if (value) {
        chorusOn = true;
        button2State = true;
        gpio_put(LED2, button2State);
      } else {
        chorusOn = false;
        button2State = false;
        gpio_put(LED2, button2State);
      }
      break;
    case 17:
      if (value) {
        delayOn = true;
        button3State = true;
        gpio_put(LED3, button3State);
      } else {
        delayOn = false;
        button3State = false;
        gpio_put(LED3, button3State);
      }
      break;
    case 18:
      ChorusMix = value;
      break;
    case 19:
      ChorusFreq = value;
      break;
    case 20:
      VibratoLevel = value;
      break;
    case 21:
      VibratoFreq = value;
      break;
    case 22:
      DelayTime = value * delaysize >> 7;
      break;
    case 23:
      DelayFeedback = value;
      break;
    case 24:
      DelayWetGain = value;
      break;
  }
}

void setup() {
  gpio_init(LED_BUILTIN);
  gpio_set_dir(LED_BUILTIN, GPIO_OUT);
  //gpio_put(LED_BUILTIN, true);
  gpio_init(4);
  gpio_set_dir(4, GPIO_OUT);

  //Buttons
  pinMode(SW1, INPUT_PULLUP);
  pinMode(SW2, INPUT_PULLUP);
  pinMode(SW3, INPUT_PULLUP);
  pinMode(SW4, INPUT_PULLUP);
  btn1.attachClick(handleClick1);
  btn2.attachClick(handleClick2);
  btn3.attachClick(handleClick3);
  btn4.attachClick(handleClick4);

  //leds
  gpio_init(LED1);
  gpio_set_dir(LED1, GPIO_OUT);
  gpio_init(LED2);
  gpio_set_dir(LED2, GPIO_OUT);
  gpio_init(LED3);
  gpio_set_dir(LED3, GPIO_OUT);
  gpio_init(LED4);
  gpio_set_dir(LED4, GPIO_OUT);

  TinyUSB_Device_Init(0);
  MIDI.begin(MIDI_CHANNEL_OMNI);
  MIDI.setHandleNoteOn(handleNoteOn);
  MIDI.setHandleNoteOff(handleNoteOff);
  MIDI.setHandleControlChange(handleControlChange);

  //Serial.begin(115200);
  //Serial.println("I2S simple tone");

  i2s.setBCLK(pBCLK);
  i2s.setDATA(pDOUT);
  i2s.setBitsPerSample(16);
  i2s.setLSBJFormat();
  i2s.setBuffers(3, 1024, 0);

  initWaves(S);

  // start I2S at the sample rate with 16-bits per sample
  if (!i2s.begin(Fe)) {
    //  Serial.println("Failed to initialize I2S!");
    while (1)
      ;  // do nothing
  }

  adjustLevel();

  for (int i = 0; i < MAXVOICE; i++) {
    Voices[i] = VOICE(0, 0, Fe);
  }
}

void loop() {
  if (i2s.availableForWrite()) {
    int32_t wavetri16 = 0;
    int32_t wavetri8 = 0;
    int32_t wavetri4 = 0;
    int32_t wavetriIV = 0;
    int32_t wavesin16 = 0;
    int32_t wavesin8 = 0;
    int32_t wavesin4 = 0;
    int32_t wavesinIV = 0;

    gpio_put(4, true);
    for (int i = 0; i < MAXVOICE; i++) {
      if (count == 0) {
        if (vibratoOn) Voices[i].update(VibratoLevel * lfoval1 / (512 - 2 * Voices[i].note));
        else Voices[i].update(0);
      }
      Voices[i].next();
      wavetri16 += Voices[i].wavetri16;
      wavetri8 += Voices[i].wavetri8;
      wavetri4 += Voices[i].wavetri4;
      wavetriIV += Voices[i].wavetriIV;
      wavesin16 += Voices[i].wavesin16;
      wavesin8 += Voices[i].wavesin8;
      wavesin4 += Voices[i].wavesin4;
      wavesinIV += Voices[i].wavesinIV;
    }
    wavetri = (Level[0] * wavetri16 + Level[1] * wavetri8 + Level[2] * wavetri4) / Level[6];
    wavetri = (127 * wavetri + Level[3] * wavetriIV) / Level[7];

    wavesin = (Level[0] * wavesin16 + Level[1] * wavesin8 + Level[2] * wavesin4) / Level[6];
    wavesin = (127 * wavesin + Level[3] * wavesinIV) / Level[7];

    wave = (Level[4] * wavesin + Level[5] * wavetri) >> 8;

    if (delayOn) {
      Delayprt = (Delayprt + 1) % DelayTime;
      waveDelay = Delayline[Delayprt];
      Delayline[Delayprt] = wave + (waveDelay * DelayFeedback >> 7);
    } else waveDelay = 0;

    wave += waveDelay * DelayWetGain >> 7;

    if (chorusOn) {
      Chorusprt++;
      Chorusline[Chorusprt] = wave;
      Chorus = ((Chorusline[(Chorusprt - (lfoval2 + 128)) & 255] + Chorusline[(Chorusprt - (128 - lfoval2)) & 255]) * ChorusMix) >> 8;
    } else Chorus = 0;

    waveL = wave - Chorus;
    waveR = wave + Chorus;

    if (waveL > 32767) waveL = 32767;
    else if (waveL < -32768) waveL = -32768;
    if (waveR > 32767) waveR = 32767;
    else if (waveR < -32768) waveR = -32768;

    i2s.write16(waveL, waveR);

    if (count <= 0) {
      lfo1prt += VibratoFreq;
      lfo2prt += ChorusFreq;
      lfoval1 = lfosin[lfo1prt];
      lfoval2 = lfosin[lfo2prt];
      /*Amp += (Vel - Amp) / EnvTime;
      Ampi = uint8_t(Amp);
      if (LastNote >= 0 && Amp == 0) {
        Voices[LastNote].noteOFF(); 
        LastNote  = -1;
      } */
      count = razCount;
    }
    count--;
  }
  gpio_put(4, false);
}

void loop1() {
  int8_t nbpot;
  MIDI.read();
  if (msec >= 20) {
    msec = 0;
    btn1.tick();
    btn2.tick();
    btn3.tick();
    btn4.tick();
    nbpot = pot.update();
    if (nbpot >= 0) {
      if (nbpot < 6) handleControlChange(0, 9 + nbpot, pot.get());
      else if (nbpot == 6) handleControlChange(0, 18, pot.get());
      else if (nbpot == 7) handleControlChange(0, 24, pot.get());
    }
  }
}
