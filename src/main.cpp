/*
 * STARDRIVER V0.0.1
 * A hardware instrument controller for RP2040/Pico boards.
 * Written by Ash
 * https://ashiecorner.xyz
 */

#include <Arduino.h>
#include "Config.h"
#include "Instruments/Instrument.h"
#include "Controllers/Controller.h"

#ifdef INSTRUMENT_FDD
#include "Instruments/FDD.h"
Instrument *instrument = new instruments::FDD();
#endif

#ifdef CTRL_MIDI
#include "Controllers/MidiController.h"
MidiController controller = MidiController(instrument);
#endif

void setup() {
  instrument->setup();

  controller.setup();
}

void loop() {
  controller.playMidi();
}