/*
 * Speaker.cpp
 * Speaker instrument main file
 */

#include "Instrument.h"
#include "Speaker.h"

namespace instruments {

    // Array that keeps track of the current state of each speaker pin
    int Speaker::pinState[NUM_SPK] = {};

    // Current note period assigned to each speaker, 0 being off.
    unsigned int Speaker::spkPeriod[NUM_SPK] = {};

    // Current duty cycle assigned to each speaker.
    unsigned int Speaker::dutyCycle[NUM_SPK] = {};

    // Tracks the tick counter for each speaker
    unsigned int Speaker::spkTickCount[NUM_SPK] = {};

    // Original note period before pitch bending events
    unsigned int Speaker::originalPeriod[NUM_SPK] = {};

    // PWM audio devices for each speaker
    //PWMAudio Speaker::pwmDevices[NUM_SPK] = {};

    // Speaker initalization code

    void Speaker::setup() {
        // initalize speaker(s)
        for (int i=0; i < NUM_SPK; i++) {
            pinMode(SPK_PINS[i], OUTPUT);
            //pwmDevices[i] = PWMAudio(SPK_PINS[i]);
        }

        // initalize all arrays (i hate c++ sometimes)
        for (int i = 0; i < NUM_SPK; i++) {
          spkPeriod[i] = 0;
          dutyCycle[i] = 1;
          spkTickCount[i] = 0;
          originalPeriod[i] = 0;
          pinState[i] = LOW;
        }

        // TODO: make speakerr work

        #ifdef INIT_SOUND
        delay(500);
        initSound(0xFF); // startup sound on all speakers
        delay(1000);
        #endif
        stopAll();
        delay(1000);

    }

    void Speaker::initSound(byte spkNum) {
      unsigned int chargeNotes[] = {
          noteDoubleTicks[46],
          noteDoubleTicks[42],
          noteDoubleTicks[41],
          noteDoubleTicks[34],
          noteDoubleTicks[27],
          noteDoubleTicks[30],
          noteDoubleTicks[34],
          noteDoubleTicks[37],
          0
      };
      if (spkNum == 0xFF) { // if address is 0xFF (255) startup sound on all drives
      byte i = 0;
      unsigned long lastRun = 0;
      while(i < 9) {
        if (millis() - 175 > lastRun) {
          lastRun = millis();
          for (byte d = 0; d < NUM_SPK; d++) {
          spkPeriod[d] = chargeNotes[i];
          }
          i++;
        }
      }} else {
      byte i = 0;
      unsigned long lastRun = 0;
      while(i < 9) {
        if (millis() - 175 > lastRun) {
          lastRun = millis();
          spkPeriod[spkNum] = chargeNotes[i++];
        }
      }}
    }

    // Controller message handling
    void Speaker::ctrl_reset() {
      stopAll();
    }

    void Speaker::ctrl_stop() {
      stopAll();
    }

    // Speaker message handling

    void Speaker::dev_reset(uint8_t devAddress) {
        reset(devAddress);
    }

    void Speaker::midi_noteOn(uint8_t devAddress, uint8_t message[]) {
        if (message[0] <= MAX_SPK_NOTE) {
            spkPeriod[devAddress] = originalPeriod[devAddress] = noteDoubleTicks[message[0]];
        }
    }

    void Speaker::midi_noteOff(uint8_t devAddress, uint8_t message[]) {
        spkPeriod[devAddress] = originalPeriod[devAddress] = 0;
    }

    void Speaker::midi_pitchBend(uint8_t devAddress, uint8_t message[]) {
        // A value from -8192 to 8191 representing the pitch deflection
        int16_t bendDeflection = message[0] << 8 | message[1];

        // A whole octave of bend would double the frequency (halve the the period) of notes
        // Calculate bend based on BEND_OCTAVES from MoppyInstrument.h and percentage of deflection
        //currentPeriod[subAddress] = originalPeriod[subAddress] / 1.4;
        spkPeriod[devAddress] = originalPeriod[devAddress] / pow(2.0, BEND_OCT * (bendDeflection / (float)8192));
    }

    void Speaker::midiEvent(uint8_t subAddress, uint8_t command, uint8_t message[]) {
    }

    // Speaker timing and driving functions

    #pragma GCC push_options
    #pragma GCC optimize("Ofast") // optimize the shit out of this since it's a timing interrupt function
    void Speaker::tick() {
      // On every drive, count the ticks that have passed and see if that's over the note period, if so, toggle the drive pins.
      for (int s = 0; s < NUM_SPK; s++) {
          if (spkPeriod[s] > 0) { // if note period isnt 0 (active note playing)
              spkTickCount[s]++;
              if (spkTickCount[s] >= spkPeriod[s]) {
                  togglePin(s, SPK_PINS[s]);
                  spkTickCount[s] = 0;
              }
          }
      }
    }
    
    void Speaker::togglePin(byte spkNum, byte pin) {
        //Pulse the control pin
        digitalWrite(pin, pinState[spkNum]);
        pinState[spkNum] = ~pinState[spkNum];
    }
    #pragma GCC pop_options
    
    // Immediately stops all speakers playback and sets pins to low
    void Speaker::stopAll() {
      for (byte d=0;d<NUM_SPK;d++) {
        spkPeriod[d] = 0;
        digitalWrite(SPK_PINS[d], LOW); // resets pin to low state
        pinState[d] = LOW; // resets pin tracking to low state 
      }
    }
    
    //For a given speaker number, stops playback and sets speaker pin to low
    void Speaker::reset(byte spkNum)
    {
      spkPeriod[spkNum] = 0; // Stop current note
      digitalWrite(SPK_PINS[spkNum], LOW); // resets pin to low state
      pinState[spkNum] = LOW; // resets pin tracking to low state 
    }
}