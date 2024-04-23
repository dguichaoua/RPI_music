
/*
       /$$   /$$ /$$$$$$$$ /$$       /$$$$$$  /$$$$$$   /$$$$$$         /$$$$$$  /$$   /$$ /$$$$$$$$          
      | $$  | $$| $$_____/| $$      |_  $$_/ /$$__  $$ /$$__  $$       /$$__  $$| $$$ | $$| $$_____/          
      | $$  | $$| $$      | $$        | $$  | $$  \ $$| $$  \__/      | $$  \ $$| $$$$| $$| $$                
      | $$$$$$$$| $$$$$   | $$        | $$  | $$  | $$|  $$$$$$       | $$  | $$| $$ $$ $$| $$$$$             
      | $$__  $$| $$__/   | $$        | $$  | $$  | $$ \____  $$      | $$  | $$| $$  $$$$| $$__/             
      | $$  | $$| $$      | $$        | $$  | $$  | $$ /$$  \ $$      | $$  | $$| $$\  $$$| $$                
      | $$  | $$| $$$$$$$$| $$$$$$$$ /$$$$$$|  $$$$$$/|  $$$$$$/      |  $$$$$$/| $$ \  $$| $$$$$$$$          
      |__/  |__/|________/|________/|______/ \______/  \______/        \______/ |__/  \__/|________/          
                                                                                                              
                                                                                                              
                                                                                                              
                                       /$$     /$$                           /$$                              
                                      | $$    | $$                          |__/                              
        /$$$$$$$ /$$   /$$ /$$$$$$$  /$$$$$$  | $$$$$$$   /$$$$$$   /$$$$$$$ /$$ /$$$$$$$$  /$$$$$$   /$$$$$$ 
       /$$_____/| $$  | $$| $$__  $$|_  $$_/  | $$__  $$ /$$__  $$ /$$_____/| $$|____ /$$/ /$$__  $$ /$$__  $$
      |  $$$$$$ | $$  | $$| $$  \ $$  | $$    | $$  \ $$| $$$$$$$$|  $$$$$$ | $$   /$$$$/ | $$$$$$$$| $$  \__/
       \____  $$| $$  | $$| $$  | $$  | $$ /$$| $$  | $$| $$_____/ \____  $$| $$  /$$__/  | $$_____/| $$      
       /$$$$$$$/|  $$$$$$$| $$  | $$  |  $$$$/| $$  | $$|  $$$$$$$ /$$$$$$$/| $$ /$$$$$$$$|  $$$$$$$| $$      
      |_______/  \____  $$|__/  |__/   \___/  |__/  |__/ \_______/|_______/ |__/|________/ \_______/|__/      
                 /$$  | $$                                                                                    
                |  $$$$$$/                                                                                    
                 \______/     
                                                                
                                                                                 v4.6 

   
 // A BlogHoskins Monstrosity @ 2019 / 2020
// https://bloghoskins.blogspot.com/
/*    
 *    v4.6 
 *    Time to fix distortion - Fixed
 *    changed update audio from 8 -> 9
 *    Change Filter Res and Cut-off from >>2 to mozzieMap -> done
 *    change lfo amount pot - check if all are still in spec with multimeter - done
 *    add resistors? - yep, done & done
 *    reduce resonance total in res pot - done
 *    
 *    v4.5
 *    LFO On/off switch added - using arduino digital pin 3
 *     
 *    v4.4 
 *    Applied advice given from Mozzi Forums - working much better now - thanks Staffan
 *    Still need to apply values to stop distortion + add on/off switch for LFO
 *     
 *    v4.3 LFO implementation
 *    LFO is working(?), but filter is interfering.  Figure it out!
 *    *replaced pin assignments 'const int' with #define -> now reassign values to stop distortion
 *    
 *    
 *    V3.1 - Minor Updates
 *   *IImproved Filter Resonance (?).  Apparantly After setting resonance, you need to call setCuttoffFreq() to hear the change.
 *   Swapped order of cut-off and resonance in code.  Filter Sounds Better now?
 *   *Increased note sustain from 10 seconds to 60 seconds
 *   *OSC OUTPUT made louder on audio update>>9; // changed down from 10 to 9 because was louder
 *   
 *   
 *   V3
 *   In this version we add a low pass filter.  A cut off pot and resonance pot are added.
 *   For this you'll need two B10k pots, and a 220ohm resistor.
 *   A3: for Resonance (center pot).  Other lugs go to ground and +5v
 *   A2: for Cut-off (center pot).   Other lugs go to ground (with 220ohm resistor) and +5v
 *   
 *   
 *   v2
 *   https://bloghoskins.blogspot.com/2020/06/helios-one-arduino-synth-part-2.html
 *   This version adds Attack & Release on the analog inputs. You'll need 2 pots. 
 *   Connect center pot pin to;
 *   A5: for Atk
 *   A4: for Release
 *   connect the other pot pins to ground and 5v. 
 *   To stop mis-triggering on the atk I have x2 1k resistors in parallel (amounting 
 *   to 500 ohms resistance) on the ground input of the atk pot. you could put two 
 *   200(ish)ohm resisters in series instead, or play with the code...  maybe set the
 *   int val of the atkVal to something over 20 instead of 0?
 *   
 *   
 *   v1
 *   http://bloghoskins.blogspot.com/2019/07/helios-one-arduino-synth-part-1.html
 *   This vesrion of code lets you set between SQR and SAW wave with a 
 *   switch (input 2 on arduino)
 *   MIDI is working.
 *   You'll need to install the MIDI & Mozzi arduino libraries to get it to work.
*/

/*  
    #define EXTERNAL_AUDIO_OUTPUT true should be uncommented in mozzi_config.h.
    #define AUDIO_CHANNELS STEREO should be set in mozzi_config.h


    Circuit: 
    
    PT8211   //  Connect to:
    -------       -----------
    Vdd           V+
    BCK           4
    WS            5
    DIN           7
    GND           GND
    L/R           to audio outputs

    Mozzi documentation/API
    https://sensorium.github.io/Mozzi/doc/html/index.html

    Mozzi help/discussion/announcements:
    https://groups.google.com/forum/#!forum/mozzi-users

    Tim Barrass 2012, CC by-nc-sa.
    T. Combriat 2020, CC by-nc-sa.
*/

//#include <Adafruit_TinyUSB.h>
#include <MIDI.h>
#include <hardware/gpio.h>
//#include "hardware/adc.h"
#include "pin.h"
#include <OneButton.h>
#include <elapsedMillis.h>

#include <MozziGuts.h>
#include <mozzi_midi.h>
#include <Oscil.h>
#include <tables/cos2048_int8.h>  // table for Oscils to play
#include <ADSR.h>
#include <ResonantFilter.h>  // You need this for the LPF
#include <AutoMap.h>        // to track and reassign the pot values

#include <tables/saw2048_int8.h>               // saw table for oscillator
#include <tables/square_no_alias_2048_int8.h>  // square table for oscillator

//*****************LFO******************************************************************************************
#include <tables/cos2048_int8.h>  // for filter modulation
#include <StateVariable.h>
#include <mozzi_rand.h>  // for rand()
#include <AutoMap.h>     // To track pots **************************************************************************

#define resADC 4095

//********ADD LFO FILTER MODULATION OSC*************************************************************************
Oscil<COS2048_NUM_CELLS, CONTROL_RATE> kFilterMod(COS2048_DATA);
StateVariable<NOTCH> svf;  // can be LOWPASS, BANDPASS, HIGHPASS or NOTCH

//// Set up LFO Rate Pot****************************************************************************************
#define LFOratePot 4                      // select the input pin for the potentiometer
AutoMap LFOratePotMap(0, resADC , 40, 750);  // LFO Rate mapped 25 to 1300 (up from 5 - 1023)
//***************END********************************************************************************************

//// ****************Set up LFO Res Pot*************************************************************************
#define LFOresPot 5                      // select the input pin for the potentiometer
AutoMap LFOresPotMap(0, resADC, 40, 180);  // 0-255 val distorted, 2-212 within range - find better value?
//***************END********************************************************************************************

//Create an instance of a low pass filter
LowPassFilter lpf;

// USB MIDI object
//Adafruit_USBD_MIDI usb_midi;
// Create a new instance of the Arduino MIDI Library,
// and attach usb_midi as the transport.
//MIDI_CREATE_INSTANCE(Adafruit_USBD_MIDI, usb_midi, MIDI);
MIDI_CREATE_DEFAULT_INSTANCE();

//#define WAVE_SWITCH 2        // switch for switching waveforms
//#define LFO_ON_OFF_SWITCH 3  // switch for LFO ON/OFF

// Set up Attack & Decay Envelope
#define atkPot 0  // select the input pin for the potentiometer
//int atkVal = 0;       // variable to store the value coming from the pot
AutoMap atkPotMap(0, resADC, 0, 3000);  // remap the atk pot to 3 seconds
#define dkyPot 1                      // select the input pin for the potentiometer
//int dkyVal = 0;       // variable to store the value coming from the pot
AutoMap dkyPotMap(0, resADC, 0, 3000);  // remap the atk pot to 3 seconds

//*******CUT-OFF POT***********
#define cutoffPot 2                      // cut-off pot will be on A2
AutoMap cutOffPotMap(0, resADC, 20, 250);  // 0 - 255

//*******RESONANCE POT***********
#define resPot 3                          // resonance pot will be on A2
AutoMap resFreqPotMap(0, resADC, 40, 210);  // 0 - 255

// use #define for CONTROL_RATE, not a constant
#define CONTROL_RATE 128  // powers of 2 please

// audio sinewave oscillator
Oscil<SAW2048_NUM_CELLS, AUDIO_RATE> oscil1;               //Saw Wav
Oscil<SQUARE_NO_ALIAS_2048_NUM_CELLS, AUDIO_RATE> oscil2;  //Sqr wave

// envelope generator
ADSR<CONTROL_RATE, AUDIO_RATE> envelope;

#define LED LED_BUILTIN  // Internal LED lights up if MIDI is being received

bool button1State = false;
bool button2State = false;
bool button3State = false;
bool button4State = false;

bool sensorVal = false;
bool LFOsensorVal = false;

int8_t nbpot = -1;

elapsedMillis msec = 0;

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

// Handler function for a single click:
static void handleClick1() {
  if (button1State) {
    button1State = false;
  } else {
    button1State = true;
  }
  digitalWrite(LED1, button1State);
  sensorVal = button1State;
}
static void handleClick2() {
  if (button2State) {
    button2State = false;
  } else {
    button2State = true;
  }
  digitalWrite(LED2, button2State);
  LFOsensorVal = button2State;
}
static void handleClick3() {
  if (button3State) {
    button3State = false;
  } else {
    button3State = true;
  }
  digitalWrite(LED3, button3State);
}
static void handleClick4() {
  if (button4State) {
    HandleNoteOff(0, 69/2, 0);
    button4State = false;
  } else {
    HandleNoteOn(0, 69/2, 127);
    button4State = true;
  }
  digitalWrite(LED4, button4State);
}

void HandleNoteOn(byte channel, byte note, byte velocity) {
  oscil1.setFreq(mtof(float(note)));
  envelope.noteOn();
  digitalWrite(LED, HIGH);
}

void HandleNoteOff(byte channel, byte note, byte velocity) {
  envelope.noteOff();
  digitalWrite(LED, LOW);
}

void Pmux(uint8_t number) {
  if (number == 0) {
    digitalWrite(MUX1, false);
    digitalWrite(MUX2, false);
    digitalWrite(MUX3, false);
  }
  if (number == 4) {
    digitalWrite(MUX1, true);
    digitalWrite(MUX2, false);
    digitalWrite(MUX3, false);
  }
  if (number == 1) {
    digitalWrite(MUX1, false);
    digitalWrite(MUX2, true);
    digitalWrite(MUX3, false);
  }
  if (number == 5) {
    digitalWrite(MUX1, true);
    digitalWrite(MUX2, true);
    digitalWrite(MUX3, false);
  }
  if (number == 2) {
    digitalWrite(MUX1, false);
    digitalWrite(MUX2, false);
    digitalWrite(MUX3, true);
  }
  if (number == 6) {
    digitalWrite(MUX1, true);
    digitalWrite(MUX2, false);
    digitalWrite(MUX3, true);
  }
  if (number == 3) {
    digitalWrite(MUX1, false);
    digitalWrite(MUX2, true);
    digitalWrite(MUX3, true);
  }
  if (number == 7) {
    digitalWrite(MUX1, true);
    digitalWrite(MUX2, true);
    digitalWrite(MUX3, true);
  }
  // allow 50 us for signals to stablize
  delayMicroseconds(100);
}


void setup() {
  //  Serial.begin(9600); // see the output
  pinMode(LED, OUTPUT);  // built-in arduino led lights up with midi sent data

  //Mux
  pinMode(MUX1, OUTPUT);
  pinMode(MUX2, OUTPUT);
  pinMode(MUX3, OUTPUT);

  //TinyUSB_Device_Init(0);
  MIDI.begin(MIDI_CHANNEL_OMNI);
  // Connect the HandleNoteOn function to the library, so it is called upon reception of a NoteOn.
  MIDI.setHandleNoteOn(HandleNoteOn);    // Put only the name of the function
  MIDI.setHandleNoteOff(HandleNoteOff);  // Put only the name of the function
                                         // Initiate MIDI communications, listen to all channels (not needed with Teensy usbMIDI)

  envelope.setADLevels(255, 64);  // A bit of attack / decay while testing
  oscil1.setFreq(110);            // default frequency

  //****************Set up LFO**************************************************************************************
  kFilterMod.setFreq(1.3f);  // Can be deleted??
  //****************End*********************************************************************************************

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
  pinMode(LED1, OUTPUT);
  pinMode(LED2, OUTPUT);
  pinMode(LED3, OUTPUT);
  pinMode(LED4, OUTPUT);

  Pmux(0);

  startMozzi(CONTROL_RATE);
}

void updateControl() {
  static int atkVal;
  static int dkyVal;
  static int resVal;
  static int cutVal;
  static int LFOrateVal;
  static int LFOresVal;
  static int muxPot = 0;

  MIDI.read();

  // set ADSR times
  if (muxPot == atkPot) {
    atkVal = mozziAnalogRead(ADC);  // read the value from the attack pot
    atkVal = atkPotMap(atkVal);
  }

  if (muxPot == dkyPot) {
    dkyVal = mozziAnalogRead(ADC);  // read the value from the decay pot
    dkyVal = dkyPotMap(dkyVal);
  }

  envelope.setTimes(atkVal, 60000, 60000, dkyVal);  // 60000 is so the note will sustain 60 seconds unless a noteOff comes
  envelope.update();
  
  //**************RESONANCE POT****************
  if (muxPot == resPot) {
    resVal = mozziAnalogRead(ADC);  // arduino checks pot value
    resVal = resFreqPotMap(resVal);
    lpf.setCutoffFreqAndResonance(cutVal, resVal);  // change the freq
  }

  //**************CUT-OFF POT****************
  if (muxPot == cutoffPot) {
    cutVal = mozziAnalogRead(ADC);  // arduino checks pot value
    cutVal = cutOffPotMap(cutVal);
    lpf.setCutoffFreqAndResonance(cutVal, resVal); // change the freq
  }

  //*****************Set up LFO control pots****************************************************************************
  if (muxPot == LFOratePot) {
    LFOrateVal = mozziAnalogRead(ADC);       // Speed of LFO
    LFOrateVal = LFOratePotMap(LFOrateVal);  // map to 25-1300
  }

  if (muxPot == LFOresPot) {
    LFOresVal = mozziAnalogRead(ADC);     // read the value from the attack pot
    LFOresVal = LFOresPotMap(LFOresVal);  // map to 10-210
  }

  //*****************************ADD LFO**********************************************************************************
  if (LFOsensorVal == HIGH)  // If switch is set to high, run this portion of code
  {
    //if (rand(CONTROL_RATE / 2) == 0) {      // about once every half second
      kFilterMod.setFreq(LFOrateVal / 64);  // choose a new modulation frequency****Replace setFreq((float)rand(255)/64)****
    //}
    int LFOrate_freq = 2200 + kFilterMod.next() * 12;  // cos oscillator, 2200 + (int8)*12
    svf.setCentreFreq(LFOrate_freq);
    svf.setResonance(LFOresVal);
  } else  // If switch not set to high, run this portion of code instead
  {
    svf.setCentreFreq(0);
    svf.setResonance(200);
  }
  //**********************************END*******************************************************************************

  if (sensorVal == HIGH)  // If switch is set to high, run this portion of code
  {
    oscil1.setTable(SAW2048_DATA);
  } else  // If switch not set to high, run this portion of code instead
  {
    oscil1.setTable(SQUARE_NO_ALIAS_2048_DATA);
  }

  muxPot++;
  if (muxPot > 5) muxPot = 0;
  Pmux(muxPot);
}


AudioOutput_t updateAudio() {
  int asig = (envelope.next() * oscil1.next()) >> 4;  // multiplying by 0-255 and then dividing by 256
  int asigSVF = svf.next(asig);                       // SVF
  int asigLPF = lpf.next(asigSVF);                    // LPF
  return StereoOutput::from16Bit(asigLPF, asigLPF);
}


void loop() {
  audioHook();
}

void loop1() {
  if (msec >= 20) {
    msec = 0;
    btn1.tick();
    btn2.tick();
    btn3.tick();
    btn4.tick();
  }
}
