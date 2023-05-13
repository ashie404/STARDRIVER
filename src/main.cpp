/* STARDRIVER v0.1 */
/* written by ash  */
/* ashiecorner.xyz */

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
  pinMode(25, OUTPUT);
  instrument->setup();

  controller.setup();
}

void loop() {
  controller.playMidi();
}