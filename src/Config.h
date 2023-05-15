/* 
 * Config.h
 * Main STARDRIVER config file.
 * Use this to enable instruments, disable instruments, assign instruments' pins, etc
 */

#ifndef STARDRIVER_SRC_CONFIG_H_
#define STARDRIVER_SRC_CONFIG_H_

#include "Instruments/Instrument.h"

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

//#define INSTRUMENT_SPK
#define NUM_SPK 1

// pins speaker(s) are attached to
const byte SPK_PINS[1] = {20};
const byte SPK_ADDR_RANGE[] = {10, 10}; // Address range that speakers use, doesn't matter what it's set to if disabled.

// Instrument types addresses are assigned to (this makes it easier for the controller to quickly find which device to send an event to)
const InstType ADDR_INST[] = {InstType::FDD, InstType::FDD, InstType::FDD, InstType::FDD, InstType::FDD, InstType::FDD, InstType::FDD, 
    InstType::FDD, InstType::FDD, InstType::Speaker};


/// Controller you will use (currently only onboard MIDI and Moppy serial supported)

//#define CTRL_MIDI
#define CTRL_SERIAL

#endif