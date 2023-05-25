/* 
 * LEDStrip.h
 * LED Lightshow effects instrument header file
 */

#ifndef STARDRIVER_INSTRUMENTS_LEDSTRIP_H_
#define STARDRIVER_INSTRUMENTS_LEDSTRIP_H_

#include <Arduino.h>
#include <Adafruit_NeoPixel.h>
#include "Instrument.h"
#include "Timer.h"
#include "../Config.h"

namespace instruments {
    class LEDStrip : public Instrument {
        public:
            Adafruit_NeoPixel strip;
            void setup();
            static void tick();

        protected:
            void ctrl_stop() override;
            void ctrl_reset() override;

            void dev_reset(uint8_t devAddress) override;
            void midi_noteOn(uint8_t devAddress, uint8_t message[]) override;
            void midi_noteOff(uint8_t devAddress, uint8_t message[]) override;
            void midi_pitchBend(uint8_t devAddress, uint8_t message[]) override;
            
            void midiEvent(uint8_t devAddress, uint8_t event, uint8_t message[]);

        private:
            static uint8_t ledState[][4];

            static void stopAll();
            void reset();
            void initDisplay();
    };
}

#endif