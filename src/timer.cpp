#include <Arduino.h>

#include "debug.h"
#include "timer.h"

Timer *timer_init(void (*cb)(void)) {
    ALLOC_STRUCT(timer, Timer);
    if (!timer) return NULL;
    timer->code = 0;
    timer->active = 0;
    timer->timeout = 10000;
    timer->updated = 0;
    timer->cb = cb;
    return timer;
}

void timer_set(Timer *timer, uint8_t code, unsigned long timeout) {
    timer->code = code;
    timer->active = 1;
    timer->timeout = timeout;
    timer->updated = millis();
}

void timer_update(Timer *timer) {
    if (!timer->active || millis() - timer->updated < timer->timeout) {
        return;
    }
    timer->active = 0;
    timer->cb();
}
