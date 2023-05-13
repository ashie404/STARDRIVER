/*
 * FDD.cpp
 * Floppy drive instrument main file
 */

#include "Instrument.h"
#include "FDD.h"

namespace instruments {

    // Minimum and maximum head positions for all drives
    unsigned int FDD::MIN_HEAD_POSITION[NUM_FDD] = {0};
    unsigned int FDD::MAX_HEAD_POSITION[NUM_FDD] = {158};

    // Array that tracks the current head position of each floppy drive.
    unsigned int FDD::currentHeadPos[NUM_FDD] = {0};

    // Array that keeps track of the current state of each floppy pin
    int FDD::pinState[NUM_FDD*2] = {LOW};

    // Current note period assigned to each drive, 0 being off.
    unsigned int FDD::drivePeriod[NUM_FDD] = {0};

    // Tracks the current tick-count for each drive (see FloppyDrives::tick() below)
    unsigned int FDD::driveTickCount[NUM_FDD] = {0};

    // Original note period before pitch bending events
    unsigned int FDD::originalPeriod[NUM_FDD] = {0};

    // FDD initalization code

    void FDD::setup() {
        // initalize step and direction pins
        for (byte i=0; i < NUM_FDD; i++) {
            pinMode(FDD_PINS[i][0], OUTPUT_12MA);
            pinMode(FDD_PINS[i][1], OUTPUT_12MA);
        }

        // reset all drives
        resetAll();
        delay(1000); // wait a bit so we dont break things

        Timer::initialize(TMR_RES, tick); // initalize timer to set resolution and attach to tick function

        delay(500);
        startupSound(0);
        delay(500);
        resetAll();
    }

    void FDD::startupSound(byte driveNum) {
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
      byte i = 0;
      unsigned long lastRun = 0;
      while(i < 10) {
        if (millis() - 175 > lastRun) {
          lastRun = millis();
          drivePeriod[driveNum] = chargeNotes[i++];
        }
      }
    }

    // FDD message handling

    void FDD::drv_reset(uint8_t devAddress) {
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
        int step_pin_tracking = driveNum*2; // the array index of the pin we're tracking (drive number x 2 for the step pin, x2+1 for direction)
        int dir_pin_tracking = step_pin_tracking+1;

        //Switch directions if end has been reached
        if (currentHeadPos[driveNum] >= MAX_HEAD_POSITION[driveNum]) {
            pinState[dir_pin_tracking] = HIGH;
            digitalWrite(direction_pin, HIGH);
        } else if (currentHeadPos[driveNum] <= MIN_HEAD_POSITION[driveNum]) {
            pinState[dir_pin_tracking] = LOW;
            digitalWrite(direction_pin, LOW);
        }

        //Update currentPosition
        if (pinState[dir_pin_tracking] == HIGH) {
            currentHeadPos[driveNum]--;
        } else {
            currentHeadPos[driveNum]++;
        }

        //Pulse the control pin
        digitalWrite(step_pin, pinState[step_pin_tracking]);
        pinState[step_pin_tracking] = ~pinState[step_pin_tracking];
    }
    #pragma GCC pop_options
    
    // Immediately stops all drives
    void FDD::stopAll() {
      for (byte d=0;d<NUM_FDD;d++) {
        drivePeriod[d] = 0;
      }
    }

    //For a given floppy number, zeros the head.
    void FDD::reset(byte driveNum)
    {
      drivePeriod[driveNum] = 0; // Stop current note

      byte stepPin = FDD_PINS[driveNum][0];
      byte dirPin = FDD_PINS[driveNum][1];
      int step_pin_tracking = driveNum*2; // the array index of the pin we're tracking (drive number x 2 for the step pin, x2+1 for direction)
      int dir_pin_tracking = step_pin_tracking+1;

      digitalWrite(dirPin,HIGH); // Go in reverse
      for (unsigned int s=0;s<MAX_HEAD_POSITION[driveNum];s+=2){ //Half max because we're stepping directly (no toggle)
        digitalWrite(stepPin,HIGH);
        digitalWrite(stepPin,LOW);
        delay(5);
      }
      currentHeadPos[driveNum] = 0;
      pinState[step_pin_tracking] = LOW;
      digitalWrite(dirPin,LOW);
      pinState[dir_pin_tracking] = LOW;
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
      for (unsigned int s=0;s<MAX_HEAD_POSITION[0];s+=2){ //Half max because we're stepping directly (no toggle); grab max from index 0
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
        int step_pin_tracking = d*2;
        currentHeadPos[d] = 0;
        pinState[step_pin_tracking] = LOW;
        digitalWrite(stepPin+1,LOW);
        pinState[step_pin_tracking+1] = LOW;
      }
    }
}