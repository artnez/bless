#include <Arduino.h>
#include <stdlib.h>

#include "ble.h"
#include "db.h"
#include "debug.h"
#include "hci.h"
#include "pin.h"

HCI *hci_init(BLE *ble, DB *db, Callback cb) {
    ALLOC_STRUCT(hci, HCI);
    if (!hci) return NULL;
    hci->ble = ble;
    hci->db = db;
    hci->cb = cb;
    hci->cycles = 0;
    hci->events = 0;
    return hci;
}

void hci_update(HCI *hci) {
    uint8_t events = 0;
    while (ble_available(hci->ble)) {
        Event *event = hci_receive(hci);
        if (!event) continue;
        events ++;
        hci->cb(event);
        event_free(event);
    }
    if (events) {
        hci->events += events;
    }
}

Event *hci_receive(const HCI *hci) {
    ALLOC_STRUCT(event, Event);
    if (!event) return NULL;
    uint8_t packet_type = ble_read(hci->ble);
    uint8_t event_code = ble_read(hci->ble);

    event->data_size = ble_read(hci->ble);
    uint8_t buffer[event->data_size];
    for (int i=0; i<event->data_size; i++) {
        buffer[i] = ble_read(hci->ble);
    }

    if (packet_type != HCI_PACKET_TYPE_EVENT) {
        INFO("Non-event packet: %#04x %#04x", packet_type, event_code);
        event_free(event);
        return NULL;
    }

    if (event_code != HCI_PACKET_CODE_LE_EXT) {
        INFO("Non LE_EXT packet: %#04x %#04x", packet_type, event_code);
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
        ALLOC(data, uint8_t, event->data_size);
        if (!data) {
            event->data_size = 0;
            event_free(event);
            return NULL;
        }
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
    ALLOC_STRUCT(data, MessageDeviceInit);
    if (!data) return;
    data->profile_role = HCI_PROFILE_ROLE_CENTRAL;
    data->max_scan_res = 5;
    memset(&data->irk, 0, sizeof(data->irk));
    memset(&data->csrk, 0, sizeof(data->csrk));
    memset(&data->sign_counter, 0, sizeof(data->sign_counter));
    data->sign_counter[0] = 0x01;

    ALLOC_STRUCT(msg, Message);
    if (!msg) {
        free(data);
        return;
    }
    msg->type = HCI_PACKET_TYPE_COMMAND;
    msg->opcode = HCI_OPCODE_INIT_DEVICE;
    msg->data_size = sizeof(MessageDeviceInit);
    msg->data = data;

    hci_send(hci, msg);
    message_free(msg);
}

void hci_start_discovery(const HCI *hci) {
    ALLOC_STRUCT(data, MessageDiscover);
    if (!data) return;
    data->mode = HCI_DISCOVERY_MODE_ALL;
    data->active_scan = 0;
    data->white_list = 0;

    ALLOC_STRUCT(msg, Message);
    if (!msg) {
        free(data);
        return;
    }
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
        DEBUG_DUMP("> data: ", msg->data, msg->data_size);
        DEBUG("");
    }
}

void hci_log_event(const Event *event) {
    DEBUG("< event");
    DEBUG("< status: %#02x", event->status);
    DEBUG("< type: %#06x", event->type);
    DEBUG("< size: %#04x", event->data_size);
    if (event->data_size > 0) {
        DEBUG_DUMP("< data: ", event->data, event->data_size);
        DEBUG("");
    }
}
