#include <Arduino.h>
#include <stdlib.h>

#include "ble.h"
#include "debug.h"
#include "hci.h"
#include "pin.h"

HCI *hci_init(BLE *ble, Callback cb) {
    HCI *hci = (HCI *) malloc(sizeof(HCI));
    hci->ble = ble;
    hci->cb = cb;
    return hci;
}

void hci_update(HCI *hci) {
    digitalWrite(PIN_LED_YELLOW, LOW);

    while (ble_available(hci->ble)) {
        digitalWrite(PIN_LED_YELLOW, HIGH);
        Event *event = hci_receive(hci);
        hci->cb(event);
    }
}

Event *hci_receive(const HCI *hci) {
    Event *event = (Event *) malloc(sizeof(Event)); 

    uint8_t packet_type = ble_read(hci->ble);
    uint8_t event_code = ble_read(hci->ble);

    event->data_size = ble_read(hci->ble);
    char data[event->data_size];
    for (int i=0; i<event->data_size; i++) {
        data[i] = ble_read(hci->ble);
    }

    int offset = 0;

    memcpy(&event->type, data + offset, sizeof(event->type));
    offset += sizeof(event->type);

    memcpy(&event->status, data + offset, sizeof(event->status));
    offset += sizeof(event->status);

    memcpy(&event->opcode, data + offset, sizeof(event->opcode));
    offset += sizeof(event->opcode);

    event->data_size -= offset;
    if (event->data_size > 0) {
        memcpy(event->data, data + 5, event->data_size);
    }

    if (packet_type != HCI_PACKET_TYPE_EVENT) {
        log_debug(F("Received non-event packet: %#04x"), packet_type);
        return NULL;
    }

    if (event_code != HCI_PACKET_CODE_LE_EXT) {
        log_debug(F("Received non-le event code: %#04x"), event_code);
        return NULL;
    }

    #ifdef BLESS_DEBUG
    hci_log_event(event);
    #endif

    return event;
}

void hci_send(const HCI *hci, const Message *msg) { 
    #ifdef BLESS_DEBUG
    hci_log_message(msg);
    #endif

    ble_write(hci->ble, (uint8_t *) msg, sizeof(Message) - sizeof(msg->data));
    ble_write(hci->ble, (uint8_t *) msg->data, msg->data_size);
}

void hci_device_init(const HCI *hci) {
    MessageDeviceInit data;
    data.profile_role = HCI_PROFILE_ROLE_CENTRAL;
    data.max_scan_res = 5;
    memset(data.irk, 0, sizeof(data.irk));
    memset(data.csrk, 0, sizeof(data.irk));
    data.sign_counter[0] = 0x01;
    memset(data.sign_counter+1, 0, sizeof(data.sign_counter)-1);

    Message *msg = (Message *) malloc(sizeof(Message));
    msg->type = HCI_PACKET_TYPE_COMMAND;
    msg->opcode = HCI_OPCODE_INIT_DEVICE;
    msg->data_size = sizeof(data);
    msg->data = &data;

    hci_send(hci, msg);
}

void hci_log_message(const Message *msg) {
    log_writeln(F(""));
    log_writeln(F("> message"));
    log_writeln(F("> type: %#04x"), msg->type);
    log_writeln(F("> code: %#06x"), msg->opcode);
    log_writeln(F("> size: %d"), msg->data_size);
    if (msg->data_size > 0) {
        log_write(F("> data: "));
        hci_log_data(msg->data, msg->data_size);
        log_writeln(F(""));
    }
    log_writeln(F(""));
}

void hci_log_event(const Event *event) {
    log_writeln(F(""));
    log_writeln(F("< event"));
    log_writeln(F("< status: %#02x"), event->status);
    log_writeln(F("< type: %#04x"), event->type);
    log_writeln(F("< opcode: %#06x"), event->opcode);
    log_writeln(F("< size: %#04x"), event->data_size);
    if (event->data_size > 0) {
        log_write(F("< data: "));
        hci_log_data(event->data, event->data_size);
        log_writeln(F(""));
    }
    log_writeln(F(""));
}

void hci_log_data(const void *data, uint8_t size) {
    char *p = (char *) data;
    for (int i=0; i<size; i++) {
        log_write(F("%02x "), *(p + i));
    };
}
