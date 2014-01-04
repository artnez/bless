#ifndef __BLESS_STATE_H
#define __BLESS_STATE_H

typedef struct {
    uint8_t code;
    unsigned long timeout;
    unsigned long updated;
} State;

State *state_init();
void state_set(State *state, uint8_t code, unsigned long timeout);
int state_expired(State *state);

#endif
