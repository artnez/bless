#include <Arduino.h>
#include <AltSoftSerial.h>
#include <string.h>

#include "ble.h"
#include "db.h"
#include "debug.h"
#include "hci.h"
#include "pin.h"
#include "timer.h"

#define BAUD_RATE 57600

#define TIMEOUT_INITIALIZE 5000
#define TIMEOUT_DISCOVER   30000

#define STATE_INITIALZING 1 << 0
#define STATE_DISCOVERING 1 << 1

void event(const Event *event);
void timeout();

AltSoftSerial BLEMini;
BLE *ble;
HCI *hci;
Timer *timer;

void setup() {
    log_init(&Serial, BAUD_RATE);
    INFO("Starting up...");

    pinMode(PIN_LED_ON, OUTPUT);
    pinMode(PIN_LED_RX, OUTPUT);
    pinMode(PIN_LED_TX, OUTPUT);

    ble = ble_init(&BLEMini, BAUD_RATE);
    hci = hci_init(ble, db_init(), event);
    timer = timer_init(timeout);

    INFO("Initialize CC2540...");
    state_set(state, STATE_INITIALZING, TIMEOUT_INITIALIZE);
    hci_device_init(hci);
}

void loop() {
    hci_update(hci);
    timeout();
}

void event(const Event *event) {
    Device *device;
    switch (event->type) {
        case HCI_EVENT_DEVICE_INIT_DONE:
            INFO("CC250 initialized.");
            INFO("Start discovery...");
            state_set(state, STATE_DISCOVERING, TIMEOUT_DISCOVER);
            digitalWrite(PIN_LED_ON, HIGH);
            hci_start_discovery(hci);
            break;
        case HCI_EVENT_DEVICE_INFORMATION:
            device = (Device *) event->data;
            INFO_DATA("Device Address: ", &device->addr, sizeof(device->addr));
            INFO("Device Response: %#04x", device->event_type);
            INFO("Device RSSI: %d", device->rssi);
            INFO("Device Stored: %d", hci_device_store(hci, device));
            break;
        case HCI_EVENT_DEVICE_DISCOVERY_DONE:
            INFO("Discovery complete.");
            INFO("Memory: %d", mem_available());
            INFO("Start discovery...");
            delay(1000);
            state_set(state, STATE_DISCOVERING, TIMEOUT_DISCOVER);
            hci_start_discovery(hci);
            break;
    }
}

void timeout() {
    if (!state_expired(state)) {
        return;
    }
    INFO("State %d timeout in %ldms. Reset...", state->code, state->timeout);
    delay(1000);
    asm volatile ("jmp 0");
}
