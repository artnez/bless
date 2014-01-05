#include <Arduino.h>
#include <stdlib.h>

#include "ble.h"
#include "db.h"
#include "debug.h"
#include "hci.h"
#include "pin.h"

HCI *hci_init(BLE *ble, DB *db, Callback cb) {
    HCI *hci = (HCI *) malloc(sizeof(HCI));
    hci->ble = ble;
    hci->db = db;
    hci->cb = cb;
    return hci;
}

void hci_update(HCI *hci) {
    if (!ble_available(hci->ble)) {
        return;
    }

    Event *event = hci_receive(hci);
    if (!event) {
        return;
    }

    hci->cb(event);
    event_free(event);
}

Event *hci_receive(const HCI *hci) {
    Event *event = (Event *) malloc(sizeof(Event)); 

    uint8_t packet_type = ble_read(hci->ble);
    uint8_t event_code = ble_read(hci->ble);

    event->data_size = ble_read(hci->ble);
    uint8_t buffer[event->data_size];
    for (int i=0; i<event->data_size; i++) {
        buffer[i] = ble_read(hci->ble);
    }

    if (packet_type != HCI_PACKET_TYPE_EVENT) {
        event_free(event);
        return NULL;
    }

    if (event_code != HCI_PACKET_CODE_LE_EXT) {
        event_free(event);
        return NULL;
    }

    int offset = 0;

    memcpy(&event->type, buffer + offset, sizeof(event->type));
    offset += sizeof(event->type);

    memcpy(&event->status, buffer + offset, sizeof(event->status));
    offset += sizeof(event->status);

    event->data_size -= offset;
    if (event->data_size > 0) {
        uint8_t *data = (uint8_t *) malloc(event->data_size);
        memcpy(data, buffer + offset, event->data_size);
        event->data = data;
    }

    hci_log_event(event);
    return event;
}

void hci_send(const HCI *hci, const Message *msg) { 
    hci_log_message(msg);
    ble_write(hci->ble, (uint8_t *) msg, sizeof(Message) - sizeof(msg->data));
    ble_write(hci->ble, (uint8_t *) msg->data, msg->data_size);
}

void hci_device_init(const HCI *hci) {
    MessageDeviceInit *data = (MessageDeviceInit *) malloc(sizeof(MessageDeviceInit));
    data->profile_role = HCI_PROFILE_ROLE_CENTRAL;
    data->max_scan_res = 5;
    memset(&data->irk, 0, sizeof(data->irk));
    memset(&data->csrk, 0, sizeof(data->csrk));
    memset(&data->sign_counter, 0, sizeof(data->sign_counter));
    data->sign_counter[0] = 0x01;

    Message *msg = (Message *) malloc(sizeof(Message));
    msg->type = HCI_PACKET_TYPE_COMMAND;
    msg->opcode = HCI_OPCODE_INIT_DEVICE;
    msg->data_size = sizeof(MessageDeviceInit);
    msg->data = data;

    hci_send(hci, msg);
    message_free(msg);
}

void hci_start_discovery(const HCI *hci) {
    MessageDiscover *data = (MessageDiscover *) malloc(sizeof(MessageDiscover));
    data->mode = HCI_DISCOVERY_MODE_ALL;
    data->active_scan = 1;
    data->white_list = 0;

    Message *msg = (Message *) malloc(sizeof(Message));
    msg->type = HCI_PACKET_TYPE_COMMAND;
    msg->opcode = HCI_OPCODE_DEVICE_DISCOVER;
    msg->data_size = sizeof(MessageDiscover);
    msg->data = data;

    hci_send(hci, msg);
    message_free(msg);
}

void event_free(Event *event) {
    if (event->data_size > 0) {
        free(event->data);
    }
    free(event);
}

void message_free(Message *msg) {
    if (msg->data_size > 0) {
        free(msg->data);
    }
    free(msg);
}

void hci_log_message(const Message *msg) {
    DEBUG("> message");
    DEBUG("> type: %#04x", msg->type);
    DEBUG("> code: %#06x", msg->opcode);
    DEBUG("> size: %d", msg->data_size);
    if (msg->data_size > 0) {
        DEBUG_DATA("> data: ", msg->data, msg->data_size);
        DEBUG("");
    }
}

void hci_log_event(const Event *event) {
    DEBUG("< event");
    DEBUG("< status: %#02x", event->status);
    DEBUG("< type: %#06x", event->type);
    DEBUG("< size: %#04x", event->data_size);
    if (event->data_size > 0) {
        DEBUG_DATA("< data: ", event->data, event->data_size);
        DEBUG("");
    }
}
