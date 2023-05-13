/* 
 * Config.h
 * Main STARDRIVER config file.
 * Use this to enable instruments, disable instruments, assign instruments' pins, etc
 */

#ifndef STARDRIVER_SRC_CONFIG_H_
#define STARDRIVER_SRC_CONFIG_H_

// How many octaves a full midi pitch bend will bend
#define BEND_OCT 1.0

/// Instruments you will use (currently only FDD supported)

#define INSTRUMENT_FDD
#define NUM_FDD 10 // Number of drives

// pins drives are attached to {step, direction}
const byte FDD_PINS[10][2] = {{0,1}, {2,3}, {4,5}, {6,7}, {8,9}, {10,11}, {12,13}, {14,15}, {16,17}, {18,19}};

/// Controller you will use (currently only onboard MIDI supported)

#define CTRL_MIDI

#endif