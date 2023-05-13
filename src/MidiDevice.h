/* 
 * MidiDevice 
 * Base class for any devices that accept MIDI events
 */

#ifndef STARDRIVER_SRC_MIDIDEVICE_H_
#define STARDRIVER_SRC_MIDIDEVICE_H_

#include <Arduino.h>

class MidiDevice {
    public:
        virtual void handleMidiEvent(uint8_t devAddress, uint8_t event, uint8_t message[]) {
            switch (event) {
                default:
                    midiEvent(devAddress, event, message);
                    break;
            }
        };

    protected:
        virtual void drv_reset(uint8_t devAddress){};
        virtual void midi_noteOn(uint8_t devAddress, uint8_t message[]){};
        virtual void midi_noteOff(uint8_t devAddress, uint8_t message[]){};
        virtual void midi_pitchBend(uint8_t devAddress, uint8_t message[]){};
        
        virtual void midiEvent(uint8_t devAddress, uint8_t event, uint8_t message[]);
};

#endif