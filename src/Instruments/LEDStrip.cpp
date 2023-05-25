/*
 * LEDStrip.cpp
 * LED Lightshow effects instrument main file
 */

#include "Instrument.h"
#include "LEDStrip.h"

namespace instruments {

    // Array that keeps track of the current color of each LED
    uint8_t LEDStrip::ledState[NUM_LED][4] = {};

    // various visualizer vars
    int wTracking = 0;

    // LED strip initalization code
    
    void LEDStrip::setup() {
        strip = Adafruit_NeoPixel(NUM_LED, LED_PIN, LED_MODE);

        strip.begin();
        strip.setBrightness(50);
        strip.show();

        #ifdef INIT_SOUND
        initDisplay();
        #endif
    }

    void LEDStrip::initDisplay() {
        for (int i = 0; i < 256; i++) {
            for (int led = 0; led <= NUM_LED; led++) {
                strip.setPixelColor(led, i <= 128 ? strip.Color(0,0,0,i) : strip.Color(0,0,0,256-i));
            }
            strip.show();
            delay(10);
        }
        
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
        wTracking += 30;
        strip.fill(strip.Color(0,0,0,wTracking), 0, NUM_LED);
        strip.show();
        // TODO: implement lighting effects on note on
    }

    void LEDStrip::midi_noteOff(uint8_t devAddress, uint8_t message[]) {
        wTracking -= 30;
        strip.fill(strip.Color(0,0,0,wTracking), 0, NUM_LED);
        strip.show();
        // TODO: implement lighting effects on note off
    }

    void LEDStrip::midi_pitchBend(uint8_t devAddress, uint8_t message[]) {
        // TODO: implement lighting effects on pitchbend
    }

    void LEDStrip::midiEvent(uint8_t subAddress, uint8_t command, uint8_t message[]) {
        // maybe have special lighting effects tied to cc events so lightshows can be made fancier?
    }

    void LEDStrip::reset() {
        strip.fill(strip.Color(0,0,0,0), 0, NUM_LED-1); // turn off all LEDs on strip (broken rn)
    }

    #pragma GCC push_options
    #pragma GCC optimize("Ofast") // optimize the shit out of this since it's a timing interrupt function
    void LEDStrip::tick() {
        // todo: implement actual visualizer stuff
    }
    #pragma GCC pop_options
}