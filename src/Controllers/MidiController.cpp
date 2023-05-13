/*
 * Basic MIDI controller for STARDRIVER that plays a midi file loaded into flash
 */

#include "MidiController.h"
#include "LittleFS.h"

MidiController::MidiController(MidiDevice *midiDevice) {
    targetDevice = midiDevice;
}

void MidiController::setup() {
    LittleFS.begin();
    File f = LittleFS.open("/Song_of_the_Ancients.mid", "r");
}

void MidiController::playMidi() {

}

