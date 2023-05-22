/*
 * EventDist.cpp
 * Event distributor instrument main file
 */

#include "Instrument.h"
#include "EventDist.h"

namespace instruments {
    // Event distributor init code
    void EventDist::setup() {
        
    }

    // Controller message handling
    void EventDist::ctrl_reset() {
      // Send reset to all instruments
    }

    void EventDist::ctrl_stop() {
      // Send stop to all instruments
    }

    // MIDI/device message handling

    void EventDist::dev_reset(uint8_t devAddress) {
        // send reset to respective instrument
    }

    void EventDist::midi_noteOn(uint8_t devAddress, uint8_t message[]) {
        // send midi note on to respective instrument
    }

    void EventDist::midi_noteOff(uint8_t devAddress, uint8_t message[]) {
        // send midi note off to respective instrument
    }

    void EventDist::midi_pitchBend(uint8_t devAddress, uint8_t message[]) {
        // send midi pitch bend to respective instrument
    }

    void EventDist::midiEvent(uint8_t subAddress, uint8_t command, uint8_t message[]) {
        // send midi event to respective instrument
    }
}