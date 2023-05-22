/* 
 * EventDist.h
 * Event distributor instrument header file
 */

#ifndef STARDRIVER_INSTRUMENTS_EVENTDIST_H_
#define STARDRIVER_INSTRUMENTS_EVENTDIST_H_

#include <Arduino.h>
#include "Instrument.h"
#include "../Config.h"

namespace instruments {
    class EventDist : public Instrument {
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
    };
}

#endif