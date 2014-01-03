#ifndef __BLESS_HCI_H
#define __BLESS_HCI_H

#include <stdint.h>

#include "ble.h"

typedef struct {
    uint8_t type;
    uint8_t event_code;
    uint8_t data_size;
    uint8_t event_type;
    uint8_t status;
} Event;

typedef struct {
    uint8_t type;
    uint16_t opcode;
    uint8_t size;
    void *fields;
} Message;

typedef struct {
    uint8_t profile_role;
    uint8_t max_scan_res;
    uint8_t irk[16];
    uint8_t csrk[16];
    uint8_t sign_counter[4];
} DeviceInitFields;

typedef void (*Callback)(const Event *event);

typedef struct {
    BLE *ble;
    Callback cb;
} HCI;

HCI *hci_init(BLE *ble, Callback cb);
void hci_update(HCI *hci);
void hci_message_send(const HCI *hci, const Message *msg);
size_t hci_message_size(const Message *msg);
void hci_device_init(const HCI *hci);

void hci_log_message(const Message *msg);
void hci_log_message_fields(const Message *msg);

#endif
