/* 
 * FDD.h
 * Floppy disk drive instrument header file
 */

#ifndef STARDRIVER_INSTRUMENTS_FDD_H_
#define STARDRIVER_INSTRUMENTS_FDD_H_

#include <Arduino.h>
#include "Instrument.h"
#include "Timer.h"
#include "../Config.h"

namespace instruments {
    class FDD : public Instrument {
        public:
            void setup();
            static void tick();

        protected:
            void ctrl_stop() override;
            void ctrl_reset() override;

            void dev_reset(uint8_t devAddress) override;
            void midi_noteOn(uint8_t devAddress, uint8_t message[]) override;
            void midi_noteOff(uint8_t devAddress, uint8_t message[]) override;
            void midi_pitchBend(uint8_t devAddress, uint8_t message[]) override;
            
            void midiEvent(uint8_t devAddress, uint8_t event, uint8_t message[]);

        private:
            static unsigned int minHeadPos[];
            static unsigned int maxHeadPos[];
            static unsigned int currentHeadPos[];
            static int pinState[];
            static unsigned int drivePeriod[];
            static unsigned int driveTickCount[];
            static unsigned int originalPeriod[];

            // Max note the drives are allowed to play.
            static const byte MAX_FDD_NOTE = 128;

            static void resetAll();
            static void togglePin(byte driveNum, byte step_pin, byte direction_pin);
            static void stopAll();
            static void reset(byte driveNum);
            static void initSound(byte driveNum);
    };
}

#endif