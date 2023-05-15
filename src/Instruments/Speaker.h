/* 
 * Speaker.h
 * Speaker instrument header file
 */

#ifndef STARDRIVER_INSTRUMENTS_SPEAKER_H_
#define STARDRIVER_INSTRUMENTS_SPEAKER_H_

#include <Arduino.h>
#include "Instrument.h"
#include "Timer.h"
#include "../Config.h"

namespace instruments {
    class Speaker : public Instrument {
        public:
            void setup();

        protected:
            void ctrl_stop() override;
            void ctrl_reset() override;

            void dev_reset(uint8_t devAddress) override;
            void midi_noteOn(uint8_t devAddress, uint8_t message[]) override;
            void midi_noteOff(uint8_t devAddress, uint8_t message[]) override;
            void midi_pitchBend(uint8_t devAddress, uint8_t message[]) override;
            
            void midiEvent(uint8_t devAddress, uint8_t event, uint8_t message[]);

        private:
            static int pinState[];
            static unsigned int spkPeriod[];
            static unsigned int spkTickCount[];
            static unsigned int originalPeriod[];

            // Max note the speakers are allowed to play.
            static const byte MAX_SPK_NOTE = 128;

            static void togglePin(byte spkNum, byte pin);
            static void stopAll();
            static void reset(byte spkNum);
            static void tick();
            static void initSound(byte spkNum);
    };
}

#endif