#ifndef __BLESS_HCI_H
#define __BLESS_HCI_H

#include <stdint.h>

#include "ble.h"
#include "db.h"

#define HCI_PROFILE_ROLE_CENTRAL 0x08
#define HCI_PROFILE_ROLE_OBSERVER 0x02

#define HCI_PACKET_TYPE_COMMAND 0x01
#define HCI_PACKET_TYPE_EVENT 0x04
#define HCI_PACKET_CODE_LE_EXT 0xff

#define HCI_STATUS_SUCCESS 0x00

#define HCI_EVENT_COMMAND_STATUS 0x067f
#define HCI_EVENT_DEVICE_INIT_DONE 0x0600
#define HCI_EVENT_DEVICE_DISCOVERY_DONE 0x0601
#define HCI_EVENT_DEVICE_INFORMATION 0x060d

#define HCI_OPCODE_INIT_DEVICE 0xfe00
#define HCI_OPCODE_DEVICE_DISCOVER 0xfe04

#define HCI_DISCOVERY_MODE_GENERAL 0x01
#define HCI_DISCOVERY_MODE_ALL 0x03

typedef struct {
    uint8_t event_type;
    uint8_t addr_type;
    uint8_t addr[6];
    int8_t rssi;
    int8_t data_size;
} Device;

typedef struct {
    uint16_t type;
    uint8_t status;
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

typedef struct {
    uint8_t mode;
    uint8_t active_scan;
    uint8_t white_list;
} MessageDiscover;

typedef void (*Callback)(const Event *event);

typedef struct {
    BLE *ble;
    DB *db;
    Callback cb;
    uint64_t cycles;
    uint64_t events;
} HCI;

HCI *hci_init(BLE *ble, DB *db, Callback cb);
void hci_update(HCI *hci);

Event *hci_receive(const HCI *hci);
void hci_send(const HCI *hci, const Message *msg);

void hci_device_init(const HCI *hci);
void hci_start_discovery(const HCI *hci);

void event_free(Event *event);
void message_free(Message *message);

void hci_log_message(const Message *msg);
void hci_log_event(const Event *event);

#endif
