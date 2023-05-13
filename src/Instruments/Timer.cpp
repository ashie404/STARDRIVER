/*
 * Timer.cpp
 * Basic wrapper for the RP2040 hardware timer that is used to control instrument ticks.
 */

#include "RPi_Pico_TimerInterrupt.h"
#include "Timer.h"

RPI_PICO_Timer ITimer(0);

void Timer::setup(float us, void (*isr)()) {
    ITimer.attachInterruptInterval(us, (pico_timer_callback)isr);
}