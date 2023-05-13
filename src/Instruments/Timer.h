/*
 * Timer.h
 * Header class for timer that controls instrument ticks
 */

#ifndef STARDRIVER_INSTRUMENTS_TIMER_H_
#define STARDRIVER_INSTRUMENTS_TIMER_H_

class Timer {
public:
    static void initialize(float microseconds, void (*isr)());
};

#endif