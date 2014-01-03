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
    if (!ble_available(hci->ble)) {
        return;
    }

    digitalWrite(PIN_LED_YELLOW, HIGH);
}

void hci_message_send(const HCI *hci, const Message *msg) { 
    #if BLESS_DEBUG
    hci_log_message(msg);
    #endif
    hci->ble->serial->write((uint8_t *) msg, hci_message_size(msg));
}

size_t hci_message_size(const Message *msg) {
    return (size_t) (
        sizeof(msg->type) +
        sizeof(msg->opcode) +
        sizeof(msg->size) +
        msg->size
    );
}

void hci_device_init(const HCI *hci) {
    DeviceInitFields fields;
    fields.profile_role = 0x08; // central
    fields.max_scan_res = 5;
    memset(fields.irk, 0, sizeof(fields.irk));
    memset(fields.csrk, 0, sizeof(fields.irk));
    fields.sign_counter[0] = 0x01;
    memset(fields.sign_counter+1, 0, sizeof(fields.sign_counter)-1);

    Message *msg = (Message *) malloc(sizeof(Message));
    msg->type = 0x01; // command
    msg->opcode = 0x08FE; // device init
    msg->size = sizeof(fields);
    msg->fields = &fields;

    hci_message_send(hci, msg);
}

void hci_log_message(const Message *msg) {
    log_writeln(F(""));
    log_writeln(F("-- message ----------------------"));
    log_writeln(F("size: %d"), hci_message_size(msg));
    log_writeln(F("type: %#04x"), msg->type);
    log_writeln(F("code: %#06x"), msg->opcode);
    log_writeln(F("field_size: %d"), msg->size);
    log_write(F("field_data: "));
    hci_log_message_fields(msg);
    log_writeln(F(""));
    log_writeln(F("--------------------- message --"));
    log_writeln(F(""));
}

void hci_log_message_fields(const Message *msg) {
    int i;
    char *p = (char *) msg->fields;
    for (i=0; i<msg->size; i++) {
        log_write(F("%02x "), *(p + i));
    };
}
