/* Base instrument class */
/* Stores constants used by all instruments */

#ifndef STARDRIVER_INSTRUMENTS_INSTRUMENT_H_
#define STARDRIVER_INSTRUMENTS_INSTRUMENT_H_

#include "../MidiDevice.h"
#include <Arduino.h>

#define TMR_RES 40 // Timer resolution
#define TMR_RES2 (TMR_RES*2) // Double timer resolution

// The period of notes in microseconds
const unsigned int notePeriods[128] = {
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  61162, 57737, 54496, 51414, 48544, 45809, 43253, 40816, 38521, 36364, 34317, 32394, //C0 - B0
  30581, 28860, 27241, 25714, 24272, 22910, 21622, 20408, 19264, 18182, 17161, 16197, //C1 - B1
  15288, 14430, 13620, 12857, 12134, 11453, 10811, 10204, 9631, 9091, 8581, 8099, //C2 - B2
  7645, 7216, 6811, 6428, 6068, 5727, 5405, 5102, 4816, 4545, 4290, 4050, //C3 - B3
  3822, 3608, 3405, 3214, 3034, 2863, 2703, 2551, 2408, 2273, 2145, 2025, //C4 - B4
  1911, 1804, 1703, 1607, 1517, 1432, 1351, 1276, 1204, 1136, 1073, 1012, //C5 - B5
  956, 902, 851, 804, 758, 716, 676, 638, 602, 568, 536, 506, //C6 - B6
  478, 451, 426, 402, 379, 358, 338, 319, 301, 284, 268, 253, //C7 - B7
  239, 225, 213, 201, 190, 179, 169, 159, 150, 142, 134, 127,//C8 - B8
  0, 0, 0, 0, 0, 0, 0, 0
};

const unsigned int noteDoubleTicks[128] = {
    0/TMR_RES2, 0/TMR_RES2, 0/TMR_RES2, 0/TMR_RES2, 0/TMR_RES2, 0/TMR_RES2, 0/TMR_RES2, 0/TMR_RES2, 0/TMR_RES2, 0/TMR_RES2, 0/TMR_RES2, 0/TMR_RES2,
    61162/TMR_RES2, 57737/TMR_RES2, 54496/TMR_RES2, 51414/TMR_RES2, 48544/TMR_RES2, 45809/TMR_RES2, 43253/TMR_RES2, 40816/TMR_RES2, 38521/TMR_RES2, 36364/TMR_RES2, 34317/TMR_RES2, 32394/TMR_RES2, //C0 - B0
    30581/TMR_RES2, 28860/TMR_RES2, 27241/TMR_RES2, 25714/TMR_RES2, 24272/TMR_RES2, 22910/TMR_RES2, 21622/TMR_RES2, 20408/TMR_RES2, 19264/TMR_RES2, 18182/TMR_RES2, 17161/TMR_RES2, 16197/TMR_RES2, //C1 - B1
    15288/TMR_RES2, 14430/TMR_RES2, 13620/TMR_RES2, 12857/TMR_RES2, 12134/TMR_RES2, 11453/TMR_RES2, 10811/TMR_RES2, 10204/TMR_RES2, 9631/TMR_RES2, 9091/TMR_RES2, 8581/TMR_RES2, 8099/TMR_RES2, //C2 - B2
    7645/TMR_RES2, 7216/TMR_RES2, 6811/TMR_RES2, 6428/TMR_RES2, 6068/TMR_RES2, 5727/TMR_RES2, 5405/TMR_RES2, 5102/TMR_RES2, 4816/TMR_RES2, 4545/TMR_RES2, 4290/TMR_RES2, 4050/TMR_RES2, //C3 - B3
    3822/TMR_RES2, 3608/TMR_RES2, 3405/TMR_RES2, 3214/TMR_RES2, 3034/TMR_RES2, 2863/TMR_RES2, 2703/TMR_RES2, 2551/TMR_RES2, 2408/TMR_RES2, 2273/TMR_RES2, 2145/TMR_RES2, 2025/TMR_RES2, //C4 - B4
    1911/TMR_RES2, 1804/TMR_RES2, 1703/TMR_RES2, 1607/TMR_RES2, 1517/TMR_RES2, 1432/TMR_RES2, 1351/TMR_RES2, 1276/TMR_RES2, 1204/TMR_RES2, 1136/TMR_RES2, 1073/TMR_RES2, 1012/TMR_RES2, //C5 - B5
    956/TMR_RES2, 902/TMR_RES2, 851/TMR_RES2, 804/TMR_RES2, 758/TMR_RES2, 716/TMR_RES2, 676/TMR_RES2, 638/TMR_RES2, 602/TMR_RES2, 568/TMR_RES2, 536/TMR_RES2, 506/TMR_RES2, //C6 - B6
    478/TMR_RES2, 451/TMR_RES2, 426/TMR_RES2, 402/TMR_RES2, 379/TMR_RES2, 358/TMR_RES2, 338/TMR_RES2, 319/TMR_RES2, 301/TMR_RES2, 284/TMR_RES2, 268/TMR_RES2, 253/TMR_RES2, //C7 - B7
    239/TMR_RES2, 225/TMR_RES2, 213/TMR_RES2, 201/TMR_RES2, 190/TMR_RES2, 179/TMR_RES2, 169/TMR_RES2, 159/TMR_RES2, 150/TMR_RES2, 142/TMR_RES2, 134/TMR_RES2, 127/TMR_RES2, //C8 - B8
    0/TMR_RES2, 0/TMR_RES2, 0/TMR_RES2, 0/TMR_RES2, 0/TMR_RES2, 0/TMR_RES2, 0/TMR_RES2,0/TMR_RES2
};

const unsigned int noteTicks[128] = {
    0/TMR_RES, 0/TMR_RES, 0/TMR_RES, 0/TMR_RES, 0/TMR_RES, 0/TMR_RES, 0/TMR_RES, 0/TMR_RES, 0/TMR_RES, 0/TMR_RES, 0/TMR_RES, 0/TMR_RES,
    61162/TMR_RES, 57737/TMR_RES, 54496/TMR_RES, 51414/TMR_RES, 48544/TMR_RES, 45809/TMR_RES, 43253/TMR_RES, 40816/TMR_RES, 38521/TMR_RES, 36364/TMR_RES, 34317/TMR_RES, 32394/TMR_RES, //C0 - B0
    30581/TMR_RES, 28860/TMR_RES, 27241/TMR_RES, 25714/TMR_RES, 24272/TMR_RES, 22910/TMR_RES, 21622/TMR_RES, 20408/TMR_RES, 19264/TMR_RES, 18182/TMR_RES, 17161/TMR_RES, 16197/TMR_RES, //C1 - B1
    15288/TMR_RES, 14430/TMR_RES, 13620/TMR_RES, 12857/TMR_RES, 12134/TMR_RES, 11453/TMR_RES, 10811/TMR_RES, 10204/TMR_RES, 9631/TMR_RES, 9091/TMR_RES, 8581/TMR_RES, 8099/TMR_RES, //C2 - B2
    7645/TMR_RES, 7216/TMR_RES, 6811/TMR_RES, 6428/TMR_RES, 6068/TMR_RES, 5727/TMR_RES, 5405/TMR_RES, 5102/TMR_RES, 4816/TMR_RES, 4545/TMR_RES, 4290/TMR_RES, 4050/TMR_RES, //C3 - B3
    3822/TMR_RES, 3608/TMR_RES, 3405/TMR_RES, 3214/TMR_RES, 3034/TMR_RES, 2863/TMR_RES, 2703/TMR_RES, 2551/TMR_RES, 2408/TMR_RES, 2273/TMR_RES, 2145/TMR_RES, 2025/TMR_RES, //C4 - B4
    1911/TMR_RES, 1804/TMR_RES, 1703/TMR_RES, 1607/TMR_RES, 1517/TMR_RES, 1432/TMR_RES, 1351/TMR_RES, 1276/TMR_RES, 1204/TMR_RES, 1136/TMR_RES, 1073/TMR_RES, 1012/TMR_RES, //C5 - B5
    956/TMR_RES, 902/TMR_RES, 851/TMR_RES, 804/TMR_RES, 758/TMR_RES, 716/TMR_RES, 676/TMR_RES, 638/TMR_RES, 602/TMR_RES, 568/TMR_RES, 536/TMR_RES, 506/TMR_RES, //C6 - B6
    478/TMR_RES, 451/TMR_RES, 426/TMR_RES, 402/TMR_RES, 379/TMR_RES, 358/TMR_RES, 338/TMR_RES, 319/TMR_RES, 301/TMR_RES, 284/TMR_RES, 268/TMR_RES, 253/TMR_RES, //C7 - B7
    239/TMR_RES, 225/TMR_RES, 213/TMR_RES, 201/TMR_RES, 190/TMR_RES, 179/TMR_RES, 169/TMR_RES, 159/TMR_RES, 150/TMR_RES, 142/TMR_RES, 134/TMR_RES, 127/TMR_RES, //C8 - B8
    0/TMR_RES, 0/TMR_RES, 0/TMR_RES, 0/TMR_RES, 0/TMR_RES, 0/TMR_RES, 0/TMR_RES, 0/TMR_RES
};

class Instrument : public MidiDevice {
    public:
        virtual void setup() = 0;
};

#endif