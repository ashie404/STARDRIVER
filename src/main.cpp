/* STARDRIVER v0.1 */
/* written by ash  */
/* ashiecorner.xyz */

#include <Arduino.h>
#include "Config.h"
#include "Instruments/Instrument.h"

#ifdef INSTRUMENT_FDD
#include "Instruments/FDD.h"
Instrument *instrument = new instruments::FDD();
#endif

void setup() {
  pinMode(25, OUTPUT);
  instrument->setup();
}

void loop() {
  // put your main code here, to run repeatedly:
}