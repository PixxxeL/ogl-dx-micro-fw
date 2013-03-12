#include "Timer.h"

Timer::Timer() {
    start_millis = 0;
}

Timer::~Timer() {
    //
}

void Timer::start () {
    start_millis = GetTickCount();
}

DWORD Timer::get () {
    return (DWORD)(GetTickCount() - start_millis);
}

void Timer::wait (DWORD count) {
    Sleep(count);
}

void Timer::reset () {
    start_millis = GetTickCount();
}
