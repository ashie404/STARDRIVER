/*
 * FDD.cpp
 * Floppy disk drive instrument main file
 */

#include "Instrument.h"
#include "FDD.h"

namespace instruments {

    // Minimum and maximum head positions for all drives (should remain mostly the same, but there may be special instances in which these need to change)
    unsigned int FDD::minHeadPos[NUM_FDD] = {};
    unsigned int FDD::maxHeadPos[NUM_FDD] = {};

    // Array that tracks the current head position of each floppy drive.
    unsigned int FDD::currentHeadPos[NUM_FDD] = {};

    // Array that keeps track of the current state of each floppy pin
    int FDD::pinState[NUM_FDD*2] = {};

    // Current note period assigned to each drive, 0 being off.
    unsigned int FDD::drivePeriod[NUM_FDD] = {};

    // Tracks the tick counter for each drive
    unsigned int FDD::driveTickCount[NUM_FDD] = {};

    // Original note period before pitch bending events
    unsigned int FDD::originalPeriod[NUM_FDD] = {};

    // FDD initalization code

    void FDD::setup() {
        // initalize step and direction pins
        for (int i=0; i < NUM_FDD; i++) {
            pinMode(FDD_PINS[i][0], OUTPUT_12MA);
            pinMode(FDD_PINS[i][1], OUTPUT_12MA);
        }

        // initalize all arrays (i hate c++ sometimes)
        for (int i = 0; i < NUM_FDD; i++) {
          minHeadPos[i] = 0;
          maxHeadPos[i] = 158;
          currentHeadPos[i] = 0;
          drivePeriod[i] = 0;
          driveTickCount[i] = 0;
          originalPeriod[i] = 0;
        }
        for (int i = 0; i < NUM_FDD*2; i++) {
          pinState[i] = LOW;
        }
        // reset all drives
        resetAll();
        delay(1000); // wait a bit so we dont break things

        Timer::setup(TMR_RES, tick); // initalize timer to the configured resolution and attach to tick function

        #ifdef INIT_SOUND
        delay(500);
        initSound(0xFF); // init sound on all drives
        delay(1000);
        #endif
        resetAll();
        delay(1000);

    }

    void FDD::initSound(byte driveNum) {
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
      if (driveNum == 0xFF) { // if address is 0xFF (255) startup sound on all drives
      byte i = 0;
      unsigned long lastRun = 0;
      while(i < 9) {
        if (millis() - 175 > lastRun) {
          lastRun = millis();
          for (byte d = 0; d < NUM_FDD; d++) {
          drivePeriod[d] = chargeNotes[i];
          }
          i++;
        }
      }} else {
      byte i = 0;
      unsigned long lastRun = 0;
      while(i < 9) {
        if (millis() - 175 > lastRun) {
          lastRun = millis();
          drivePeriod[driveNum] = chargeNotes[i++];
        }
      }}
    }

    // Controller message handling
    void FDD::ctrl_reset() {
      resetAll();
    }

    void FDD::ctrl_stop() {
      stopAll();
    }

    // FDD message handling

    void FDD::dev_reset(uint8_t devAddress) {
        reset(devAddress);
    }

    void FDD::midi_noteOn(uint8_t devAddress, uint8_t message[]) {
        if (message[0] <= MAX_FDD_NOTE) {
            drivePeriod[devAddress] = originalPeriod[devAddress] = noteDoubleTicks[message[0]];
        }
    }

    void FDD::midi_noteOff(uint8_t devAddress, uint8_t message[]) {
        drivePeriod[devAddress] = originalPeriod[devAddress] = 0;
    }

    void FDD::midi_pitchBend(uint8_t devAddress, uint8_t payload[]) {
        // A value from -8192 to 8191 representing the pitch deflection
        int16_t bendDeflection = payload[0] << 8 | payload[1];

        // A whole octave of bend would double the frequency (halve the the period) of notes
        // Calculate bend based on BEND_OCTAVES from MoppyInstrument.h and percentage of deflection
        //currentPeriod[subAddress] = originalPeriod[subAddress] / 1.4;
        drivePeriod[devAddress] = originalPeriod[devAddress] / pow(2.0, BEND_OCT * (bendDeflection / (float)8192));
    }

    void FDD::midiEvent(uint8_t subAddress, uint8_t command, uint8_t payload[]) {
    }

    // FDD timing and driving functions

    #pragma GCC push_options
    #pragma GCC optimize("Ofast") // optimize the shit out of this since it's a timing interrupt function
    void FDD::tick() {
      // On every drive, count the ticks that have passed and see if that's over the note period, if so, toggle the drive pins.
      for (int d = 0; d < NUM_FDD; d++) {
          if (drivePeriod[d] > 0) { // if note period isnt 0 (active note playing)
              driveTickCount[d]++;
              if (driveTickCount[d] >= drivePeriod[d]) {
                  togglePin(d, FDD_PINS[d][0], FDD_PINS[d][1]);
                  driveTickCount[d] = 0;
              }
          }
      }
    }
    
    void FDD::togglePin(byte driveNum, byte step_pin, byte direction_pin) {
        int pin_tracking = (int)driveNum*2; // the array index of the pin we're tracking (drive number x 2 for the step pin, x2+1 for direction)

        //Switch directions if end has been reached
        if (currentHeadPos[driveNum] >= maxHeadPos[driveNum]) {
            pinState[pin_tracking+1] = HIGH;
            digitalWrite(direction_pin, HIGH);
        } else if (currentHeadPos[driveNum] <= minHeadPos[driveNum]) {
            pinState[pin_tracking+1] = LOW;
            digitalWrite(direction_pin, LOW);
        }

        //Update current head position accordingly
        if (pinState[pin_tracking+1] == HIGH) {
            currentHeadPos[driveNum]--;
        } else {
            currentHeadPos[driveNum]++;
        }

        //Pulse the control pin
        digitalWrite(step_pin, pinState[pin_tracking]);
        pinState[pin_tracking] = ~pinState[pin_tracking];
    }
    #pragma GCC pop_options
    
    // Immediately stops all drives
    void FDD::stopAll() {
      for (byte d=0;d<NUM_FDD;d++) {
        drivePeriod[d] = 0;
      }
    }

    //For a given drive number, zeros the head.
    void FDD::reset(byte driveNum)
    {
      drivePeriod[driveNum] = 0; // Stop current note

      byte stepPin = FDD_PINS[driveNum][0];
      byte dirPin = FDD_PINS[driveNum][1];
      int pin_tracking = driveNum*2; // the array index of the pin we're tracking (drive number x 2 for the step pin, x2+1 for direction)

      digitalWrite(dirPin,HIGH); // Go in reverse
      for (unsigned int s=0;s<maxHeadPos[driveNum];s+=2){ //Half max because we're stepping directly (no toggle)
        digitalWrite(stepPin,HIGH);
        digitalWrite(stepPin,LOW);
        delay(5);
      }
      currentHeadPos[driveNum] = 0;
      pinState[pin_tracking] = LOW;
      digitalWrite(dirPin,LOW);
      pinState[pin_tracking+1] = LOW;
    }

    // Resets all the drives
    void FDD::resetAll()
    {

      // Stop all drives and set to reverse
      for (byte d=0;d<NUM_FDD;d++) {
        byte dirPin = FDD_PINS[d][1];
        drivePeriod[d] = 0;
        digitalWrite(dirPin,HIGH);
      }

      // Reset all drives together
      for (unsigned int s=0;s<maxHeadPos[0];s+=2){ //Half max because we're stepping directly (no toggle); grab max from index 0
        for (byte d=0;d<NUM_FDD;d++) {
          byte stepPin = FDD_PINS[d][0];
          digitalWrite(stepPin,HIGH);
          digitalWrite(stepPin,LOW);
        }
        delay(5);
      }

      // Return pin and head tracking variables to ready state
      for (byte d=0;d<NUM_FDD;d++) {
        byte stepPin = FDD_PINS[d][0];
        int pin_tracking = d*2;
        currentHeadPos[d] = 0;
        pinState[pin_tracking] = LOW;
        digitalWrite(stepPin+1,LOW);
        pinState[pin_tracking+1] = LOW;
      }
    }
}