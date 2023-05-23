/*
 * LEDStrip.cpp
 * LED Lightshow effects instrument main file
 */

#include "Instrument.h"
#include "LEDStrip.h"

namespace instruments {

    // Array that keeps track of the current color of each LED
    uint8_t LEDStrip::ledState[NUM_LED][4] = {};

    // LED strip initalization code
    
    void LEDStrip::setup() {
        strip = Adafruit_NeoPixel(NUM_LED, LED_PIN, LED_MODE);

        strip.begin();
        strip.setBrightness(50);
        strip.show();
    }

    void LEDStrip::initDisplay() {
      // TODO: cool init animation for LED strip
    }

    // Controller message handling
    void LEDStrip::ctrl_reset() {
      reset();
    }

    void LEDStrip::ctrl_stop() {
      reset();
    }

    // Lighting message handling

    void LEDStrip::dev_reset(uint8_t devAddress) {
        reset();
    }

    void LEDStrip::midi_noteOn(uint8_t devAddress, uint8_t message[]) {
        // TODO: implement lighting effects on note on
    }

    void LEDStrip::midi_noteOff(uint8_t devAddress, uint8_t message[]) {
        // TODO: implement lighting effects on note off
    }

    void LEDStrip::midi_pitchBend(uint8_t devAddress, uint8_t message[]) {
        // TODO: implement lighting effects on pitchbend
    }

    void LEDStrip::midiEvent(uint8_t subAddress, uint8_t command, uint8_t message[]) {
        // maybe have special lighting effects tied to cc events so lightshows can be made fancier?
    }

    void LEDStrip::reset()
    {
        //strip->fill(strip.Color(0,0,0,0), 0, NUM_LED-1); // turn off all LEDs on strip (broken rn)
    }
}