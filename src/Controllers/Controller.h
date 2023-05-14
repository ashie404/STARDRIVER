/*
 * Controller.h
 * Header class for all controllers
 * Stores useful message bytes sent by controllers and recieved by devices.
 */

#ifndef STARDRIVER_CONTROLLERS_CONTROLLER_H_
#define STARDRIVER_CONTROLLERS_CONTROLLER_H_

// Controller message bytes
#define CTRL_RESET 0xFF
#define CTRL_START 0xFA
#define CTRL_STOP 0xFC

// Device & MIDI message bytes
#define DEV_RESET 0x0
#define MIDI_NOTEOFF 0x8
#define MIDI_NOTEON 0x9
#define MIDI_CC 0xB
#define MIDI_PITCHBEND 0xE

#endif