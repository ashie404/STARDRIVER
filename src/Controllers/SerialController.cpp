/*
 * SerialController.cpp
 * Basic serial controller for STARDRIVER, compatible with MoppyControlGUI
 */

#include "SerialController.h"

SerialController::SerialController(MidiDevice* midiDevice) {
    targetDevice = midiDevice;
}

void SerialController::setup() {
    Serial.begin(BAUDRATE);
}

/* MoppyMessages contain the following bytes:
 *  0    - START_BYTE (always 0x4d)
 *  1    - Device address (0x00 for system-wide messages)
 *  2    - Sub address (Ignored for system-wide messages)
 *  3    - Size of message body (number of bytes following this one)
 *  4    - Command byte
 *  5... - Optional payload
 */

void SerialController::readPort() {

    // If we're waiting for position 4, then we know how many bytes we're waiting for, no need
    // to start reading until they're all there.
    // TODO: This will break for large messages because the Arduino buffer size is only 64 bytes.
    // This should be optimized a bit.
    while ((messagePos != 4 && Serial.available()) || (messagePos == 4 && Serial.available() >= messageBuffer[3])) {

        switch (messagePos) {
        case 0:
            if (Serial.read() == START_BYTE) {
                messagePos = 1;
            }
            break;
        case 1:
            messageBuffer[1] = Serial.read(); // Read device address

            if (messageBuffer[1] == 0x00) {
                messagePos = 2; // System messages are for everyone, move to subAddress
                break;
            }

            // For Serial communications it's extremely unlikely that we'll be receiving messages not meant
            // for us, but this can help squash noise from being treated as a message
            if (messageBuffer[1] != 0x01) {
                messagePos = 0; // This message isn't for us
                break;
            }

            messagePos = 2; // Get subAddress next
            break;
        case 2:
            messageBuffer[2] = Serial.read(); // Read sub address

            if (messageBuffer[2] == 0x00 || (messageBuffer[2] >= 0x00 && messageBuffer[2] <= MAX_DEV_ADDR)) {
                messagePos++; // Valid subAddress, continue
                break;
            }

            messagePos = 0; // Not listening to this subAddress, skip this message
            break;
        case 3:
            messageBuffer[3] = Serial.read(); // Read message body size
            messagePos++;
            break;
        case 4:
            // Read command and message
            Serial.readBytes(messageBuffer + 4, messageBuffer[3]);

            // Call appropriate handler
            if (messageBuffer[1] == 0x00) {
                if (messageBuffer[4] == NETBYTE_SYS_PING) {
                    sendPong(); // Respond with pong if requested
                } else {
                    targetDevice->handleCtrlMessage(messageBuffer[4], &messageBuffer[5]);
                }
            } else {
                targetDevice->handleMidiEvent(messageBuffer[2], messageBuffer[4], &messageBuffer[5]);
            }

            messagePos = 0; // Start looking for a new message
        }
    }
}

void SerialController::sendPong() {
    Serial.write(pongBytes, sizeof(pongBytes));
}