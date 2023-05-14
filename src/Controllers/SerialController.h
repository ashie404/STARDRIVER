/*
 * SerialController.h
 * Basic serial controller for STARDRIVER, compatible with MoppyControlGUI
 */

#ifndef STARDRIVER_CONTROLLERS_SERIALCONTROLLER_H_
#define STARDRIVER_CONTROLLERS_SERIALCONTROLLER_H_

#include <stdint.h>
#include "Arduino.h"
#include "../Config.h"
#include "../MidiDevice.h"
#include "Controller.h"

#define BAUDRATE 57600
#define START_BYTE 0x4d

// moppy-specific bytes
#define NETBYTE_SYS_PING 0x80
#define NETBYTE_SYS_PONG 0x81
#define NETBYTE_SYS_RESET 0xff
#define NETBYTE_SYS_START 0xfa
#define NETBYTE_SYS_STOP 0xfc

#define NETBYTE_DEV_RESET 0x00
#define NETBYTE_DEV_NOTEOFF 0x08
#define NETBYTE_DEV_NOTEON 0x09
#define NETBYTE_DEV_BENDPITCH 0x0e

class SerialController {
  public:
      SerialController(MidiDevice *midiDevice);
      void begin();
      void readPort();
  private:
    MidiDevice *targetDevice;
    uint8_t messagePos = 0; // Track current message read position
    uint8_t messageBuffer[259]; // Max message length for Moppy messages is 259
    uint8_t pongBytes[8] = {0x4d, 0x00, 0x00, 0x04, 0x81, 0x01, 0x00, NUM_FDD-1};
    void sendPong();
};


#endif