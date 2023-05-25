/*
 * STARDRIVER V0.0.1
 * A hardware instrument controller for RP2040/Pico boards.
 * Written by Ash
 * https://ashiecorner.xyz
 */

#include <Arduino.h>
#include <Adafruit_NeoPixel.h>
#include "Config.h"
#include "Instruments/Instrument.h"
#include "Controllers/Controller.h"
#include <stdio.h>
#include "pico/stdlib.h"

//// Instruments

// main "instrument" 
// (a distributor instrument class used to transmit events coming in from the controllers to their respective instrument devices)
#include "Instruments/EventDist.h"
instruments::EventDist *eventDist = new instruments::EventDist();

#ifdef INSTRUMENT_FDD
#include "Instruments/FDD.h"
instruments::FDD *fdd = new instruments::FDD();
#endif

#ifdef INSTRUMENT_SPK
#include "Instruments/Speaker.h"
instruments::Speaker *speaker = new instruments::Speaker();
#endif

#ifdef INSTRUMENT_LEDSTRIP
#include "Instruments/LEDStrip.h"
instruments::LEDStrip *ledstrip = new instruments::LEDStrip();
#endif

//// Controllers

#ifdef CTRL_MIDI
#include "Controllers/MidiController.h"
MidiController controller = MidiController(eventDist);
#endif

#ifdef CTRL_SERIAL
#include "Controllers/SerialController.h"
SerialController controller = SerialController(eventDist);
#endif

// setupInstruments helper function
void setupInstruments() {

  Instrument* allInst[3] = {0,0,0};
  #ifdef INSTRUMENT_FDD
  fdd->setup();
  allInst[0] = fdd;
  #endif
  #ifdef INSTRUMENT_SPK
  speaker->setup();
  allInst[1] = speaker;
  #endif
  #ifdef INSTRUMENT_LEDSTRIP
  ledstrip->setup();
  allInst[2] = ledstrip;
  #endif

  // configure event distributor with all instruments
  eventDist->setup(allInst);
}

// woo i love janky solutions
// i guess its optimized though?
#pragma GCC push_options
#pragma GCC optimize("Ofast")
static void tickAll() {
  #ifdef INSTRUMENT_FDD
  fdd->tick();
  #endif
  #ifdef INSTRUMENT_SPK
  speaker->tick();
  #endif
  #ifdef INSTRUMENT_LEDSTRIP
  ledstrip->tick();
  #endif
}
#pragma GCC pop_options

//// Main

// CORE 0
void setup() {
  #ifndef MULTICORE
    // if multicore is not enabled, set up instruments to run on core 0
    setupInstruments();
  #endif

  controller.setup();

  // set up timer with all instruments tick functions
  Timer::setup(TMR_RES, tickAll);
}

void loop() {
  #ifdef CTRL_MIDI
  //controller.playMidi(); not yet implemented
  #endif
  #ifdef CTRL_SERIAL
  controller.readPort();
  #endif
}

#ifdef MULTICORE
// CORE 1
void setup1() {
  // set up instruments
  setupInstruments();
}

void loop1() {

}
#endif