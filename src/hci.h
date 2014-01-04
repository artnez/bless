#ifndef __BLESS_HCI_H
#define __BLESS_HCI_H

#include <stdint.h>

#include "ble.h"

#define HCI_PROFILE_ROLE_CENTRAL 0x08

#define HCI_PACKET_TYPE_COMMAND 0x01
#define HCI_PACKET_TYPE_EVENT 0x04
#define HCI_PACKET_CODE_LE_EXT 0xff

#define HCI_STATUS_SUCCESS 0x00

#define HCI_EVENT_COMMAND_STATUS 0x067f

#define HCI_OPCODE_INIT_DEVICE 0xfe00

#define hci_event(e, t, o) \
    (e->status == HCI_STATUS_SUCCESS && e->type == t && e->opcode == o)

typedef struct {
    uint8_t status;
    uint16_t type;
    uint16_t opcode;
    uint8_t data_size;
    void *data;
} Event;

typedef struct {
    uint8_t type;
    uint16_t opcode;
    uint8_t data_size;
    void *data;
} Message;

typedef struct {
    uint8_t profile_role;
    uint8_t max_scan_res;
    uint8_t irk[16];
    uint8_t csrk[16];
    uint8_t sign_counter[4];
} MessageDeviceInit;

typedef void (*Callback)(const Event *event);

typedef struct {
    BLE *ble;
    Callback cb;
} HCI;

HCI *hci_init(BLE *ble, Callback cb);
void hci_update(HCI *hci);

Event *hci_receive(const HCI *hci);
void hci_send(const HCI *hci, const Message *msg);

void hci_device_init(const HCI *hci);

void hci_log_message(const Message *msg);
void hci_log_event(const Event *event);
void hci_log_data(const void *data, uint8_t size);

#endif
