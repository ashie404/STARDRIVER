/*
 * MidiController.cpp
 * Basic MIDI controller for STARDRIVER that plays a midi file loaded into flash
 */

#include "MidiController.h"
#include "LittleFS.h"
#include "../Midi/MidiFile.h"

MidiFile midi;
std::vector<MidiFile::MidiEvent> mergedTracks;

MidiController::MidiController(MidiDevice *midiDevice) {
    targetDevice = midiDevice;
}

void MidiController::setup() {
    LittleFS.begin();
    File f = LittleFS.open("/rydeen.mid", "r");
    midi = MidiFile(f);
    
}

void MidiController::playMidi() {
    Serial.printf("boobs you have made it to the midi playing portion");
    unsigned long startTime = millis();
    unsigned long inputTime = 0;

    
    
    
}

