/* 
 * Config.h
 * Main STARDRIVER config file.
 * Use this to enable instruments, disable instruments, assign instruments' pins, etc
 */

#ifndef STARDRIVER_SRC_CONFIG_H_
#define STARDRIVER_SRC_CONFIG_H_

// whether to utilize both cores of the RP2040 or not
#define MULTICORE

// whether to play an init sound as instruments initalize themselves.
#define INIT_SOUND

// How many octaves a full midi pitch bend will bend
#define BEND_OCT 1.0

/// Instruments you will use

#define INSTRUMENT_FDD
#define NUM_FDD 10 // Number of drives
const byte FDD_ADDR_RANGE[] = {0, 9}; // Address range that floppy drives use, doesn't matter what it's set to if disabled.

// pins drive(s) are attached to {step, direction}
const byte FDD_PINS[10][2] = {{0,1}, {2,3}, {4,5}, {6,7}, {8,9}, {10,11}, {12,13}, {14,15}, {16,17}, {18,19}};

#define INSTRUMENT_SPK
#define NUM_SPK 2

// pins speaker(s) are attached to
const byte SPK_PINS[2] = {20, 21};
const byte SPK_ADDR_RANGE[] = {10, 11}; // Address range that speakers use, doesn't matter what it's set to if disabled.

#define INSTRUMENT_LEDSTRIP // enable lightshow effects (using NeoPixel/WS2812x LEDs, meant for use with RGBW neopixels)
#define LED_PIN 22 // data pin the LEDs are connected to
#define NUM_LED 48 // number of LEDs total
#define LED_MODE (NEO_GRBW + NEO_KHZ800) // led strip bitstream config
const byte LED_ADDR_RANGE[] = {0, 11}; /* LED strip address range, determines which events are sent to LEDs and which are not.
    recommended to just be the range of all addresses your regular instruments will use, 
    but can function as its own entirely independent instrument if needed. Again, doesn't matter what set to if disabled. */

/// Controller you will use (currently only onboard MIDI and Moppy serial supported)

//#define CTRL_MIDI // NOT YET IMPLEMENTED
#define CTRL_SERIAL

//// These are values not really meant to be end-user modifiable, proceed at your own risk

#define MAX_INST 3 // max number of instruments, determines size of all instruments array in event distributor

#endif