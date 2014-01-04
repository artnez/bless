#include <Arduino.h>

#include "state.h"

State *state_init() {
    State *state = (State *) malloc(sizeof(State));
    state->code = 0;
    state->timeout = 10000;
    state->updated = 0;
    return state;
}

void state_set(State *state, uint8_t code, unsigned long timeout) {
    state->code = code;
    state->timeout = timeout;
    state->updated = millis();
}

int state_expired(State *state) {
    return millis() - state->updated > state->timeout ? 1 : 0;
}
