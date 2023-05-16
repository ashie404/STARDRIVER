/*
 * MidiController.cpp
 * Basic MIDI controller for STARDRIVER that plays a midi file loaded into flash
 */

#include "MidiController.h"
#include "LittleFS.h"
#include "../Midi/MidiFile.h"

MidiFile midi;

MidiController::MidiController(MidiDevice *midiDevice) {
    targetDevice = midiDevice;
}

void MidiController::setup() {
    LittleFS.begin();
    File f = LittleFS.open("/rydeen.mid", "r");
    midi = MidiFile(f);
}

void MidiController::playMidi() {
    // TODO: Implement onboard midi player
}

