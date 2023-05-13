
#include "RPi_Pico_TimerInterrupt.h"
#include "Timer.h"

RPI_PICO_Timer ITimer(0);

void Timer::initialize(float microseconds, void (*isr)()) {
    ITimer.attachInterruptInterval(microseconds, (pico_timer_callback)isr);
}