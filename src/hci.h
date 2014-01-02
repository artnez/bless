#ifndef __BLESS_HCI_H
#define __BLESS_HCI_H

#include <stdint.h>

typedef struct {
    uint8_t type;
    uint8_t event_code;
    uint8_t data_len;
    uint8_t event_type;
    uint8_t status;
} Event;

typedef struct {
} Command;

#endif
