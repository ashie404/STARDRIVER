/*
 * MidiController.h
 * Header and class prototype for MidiController
 */

#ifndef STARDRIVER_CONTROLLERS_MIDICONTROLLER_H_
#define STARDRIVER_CONTROLLERS_MIDICONTROLLER_H_

#include <Arduino.h>
#include "../Config.h"
#include "../MidiDevice.h"
#include "Controller.h"

class MidiController {
    public:
        MidiController(MidiDevice *midiDevice);
        void setup();
        void playMidi();
    private:
        MidiDevice *targetDevice;
};

#endif