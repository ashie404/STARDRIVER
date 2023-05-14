/* 
 * MidiDevice.h
 * Header class for any devices that accept MIDI/control events from Controllers
 */

#ifndef STARDRIVER_SRC_MIDIDEVICE_H_
#define STARDRIVER_SRC_MIDIDEVICE_H_

#include <Arduino.h>
#include "Controllers/Controller.h"

class MidiDevice {
    public:
        virtual void handleCtrlMessage(uint8_t command, uint8_t payload[]) {
            switch (command) {
            
                case CTRL_START: // Controller start playing
                    ctrl_start();
                    break;
                case CTRL_STOP: // Controller stop playing
                    ctrl_stop();
                    break;
                case CTRL_RESET: // Controller reset
                    ctrl_reset();
                    break;
                default:
                    controlMessage(command, payload);
                    break;
            }
        };

        virtual void handleMidiEvent(uint8_t devAddress, uint8_t event, uint8_t message[]) {
            switch (event) {
                case DEV_RESET: // reset specific device
                    dev_reset(devAddress);
                    break;
                case MIDI_NOTEON: // midi note on
                    midi_noteOn(devAddress, message);
                    Serial.printf("FDD %d NOTE_ON\n", devAddress);
                    break;
                case MIDI_NOTEOFF: // midi note off
                    midi_noteOff(devAddress, message);
                    Serial.printf("FDD %d NOTE_OFF\n", devAddress);
                    break;
                case MIDI_PITCHBEND: // midi pitch bend
                    midi_pitchBend(devAddress, message);
                    Serial.printf("FDD %d PITCH_BEND\n", devAddress);
                    break;
                case MIDI_CC: // midi control change
                    midi_cc(devAddress, message);
                    Serial.printf("FDD %d CC\n", devAddress);
                    break;
                default:
                    midiEvent(devAddress, event, message);
                    break;
            }
        };

    protected:
        virtual void ctrl_start(){};
        virtual void ctrl_stop(){};
        virtual void ctrl_reset(){};
        virtual void controlMessage(uint8_t command, uint8_t message[]){};

        virtual void dev_reset(uint8_t devAddress){};
        virtual void midi_noteOn(uint8_t devAddress, uint8_t message[]){};
        virtual void midi_noteOff(uint8_t devAddress, uint8_t message[]){};
        virtual void midi_pitchBend(uint8_t devAddress, uint8_t message[]){};
        virtual void midi_cc(uint8_t devAddress, uint8_t message[]){};
        virtual void midiEvent(uint8_t devAddress, uint8_t event, uint8_t message[]);
};

#endif