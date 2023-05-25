/*
 * EventDist.cpp
 * Event distributor instrument main file
 */

#include "Instrument.h"
#include "EventDist.h"

namespace instruments {
    // Event distributor init code
    void EventDist::setup() {
        // setup doesn't really do anything on its own
    }
    void EventDist::setup(Instrument* allInst[]) {
        for (int i = 0; i < MAX_INST; i++) {
            _all_inst[i] = allInst[i];
        }
    }

    void EventDist::handleCtrlMessage(uint8_t command, uint8_t payload[]) {
        // send controller message to every instrument
        for (int i = 0; i < MAX_INST; i++) {
            if (_all_inst[i] != 0)
                _all_inst[i]->handleCtrlMessage(command, payload);
        }
    }

    void EventDist::handleMidiEvent(uint8_t devAddress, uint8_t event, uint8_t message[]) {
        // TODO: this is kinda janky. is there a better way to do this?
        #ifdef INSTRUMENT_FDD
        if (FDD_ADDR_RANGE[1] >= devAddress && devAddress >= FDD_ADDR_RANGE[0]) {
            _all_inst[0]->handleMidiEvent(devAddress-FDD_ADDR_RANGE[0], event, message);
        }
        #endif
        #ifdef INSTRUMENT_SPK
        if (SPK_ADDR_RANGE[1] >= devAddress && devAddress >= SPK_ADDR_RANGE[0]) {
            _all_inst[1]->handleMidiEvent(devAddress-SPK_ADDR_RANGE[0], event, message);
        }
        #endif
        #ifdef INSTRUMENT_LEDSTRIP
        if (LED_ADDR_RANGE[1] >= devAddress && devAddress >= LED_ADDR_RANGE[0]) {
            _all_inst[2]->handleMidiEvent(devAddress-LED_ADDR_RANGE[0], event, message);
        }
        #endif
    }

    // these will never be called, but c++ get angy if u dont implement them
    void EventDist::ctrl_reset() {
    }

    void EventDist::ctrl_stop() {
    }

    void EventDist::dev_reset(uint8_t devAddress) {
    }

    void EventDist::midi_noteOn(uint8_t devAddress, uint8_t message[]) {
    }

    void EventDist::midi_noteOff(uint8_t devAddress, uint8_t message[]) {
    }

    void EventDist::midi_pitchBend(uint8_t devAddress, uint8_t message[]) {
    }

    void EventDist::midiEvent(uint8_t subAddress, uint8_t command, uint8_t message[]) {
    }
}