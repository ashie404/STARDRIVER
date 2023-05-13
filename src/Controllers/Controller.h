/*
 * Base class for all controllers
 *
*/

#ifndef STARDRIVER_CONTROLLERS_CONTROLLER_H_
#define STARDRIVER_CONTROLLERS_CONTROLLER_H_

// Controller message bytes
#define CTRL_RESET 0x1
#define CTRL_START 0x2
#define CTRL_STOP 0x3

// Device & MIDI message bytes
#define DEV_RESET 0x0
#define MIDI_NOTEOFF 0x8
#define MIDI_NOTEON 0x9
#define MIDI_CC 0xB
#define MIDI_PITCHBEND 0xE

#endif