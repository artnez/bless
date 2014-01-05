#ifndef __BLESS_TIMER_H
#define __BLESS_TIMER_H

typedef struct Timer {
    uint8_t code;
    uint8_t active;
    unsigned long timeout;
    unsigned long updated;
    void (*cb)(void);
} Timer;

Timer *timer_init(void (*cb)(void));
void timer_set(Timer *timer, uint8_t code, unsigned long timeout);
void timer_update(Timer *timer);

#endif
