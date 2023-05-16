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

#ifdef INSTRUMENT_SPK
#include "Instruments/Speaker.h"
Instrument *instrument = new instruments::Speaker();
#endif

#ifdef CTRL_MIDI
#include "Controllers/MidiController.h"
MidiController controller = MidiController(instrument);
#endif

#ifdef CTRL_SERIAL
#include "Controllers/SerialController.h"
SerialController controller = SerialController(instrument);
#endif

// CORE 0
void setup() {
  pinMode(25, OUTPUT);

  controller.setup();
}

void loop() {
  digitalWrite(25, HIGH);
  #ifdef CTRL_MIDI
  //controller.playMidi();
  #endif
  #ifdef CTRL_SERIAL
  controller.readPort();
  #endif
}

// CORE 1
void setup1() {
  instrument->setup();
}

void loop1() {

}