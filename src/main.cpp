#include <Arduino.h>
#include <AltSoftSerial.h>
#include <string.h>

#include "ble.h"
#include "db.h"
#include "debug.h"
#include "hci.h"
#include "pin.h"

#define BAUD_RATE 57600

void event(const Event *event);
void status();

AltSoftSerial BLEMini;
BLE *ble;
HCI *hci;

void setup() {
    log_init(&Serial, BAUD_RATE);
    INFO("Starting up...");

    pinMode(PIN_LED_ON, OUTPUT);
    pinMode(PIN_LED_RX, OUTPUT);
    pinMode(PIN_LED_TX, OUTPUT);

    ble = ble_init(&BLEMini, BAUD_RATE);
    hci = hci_init(ble, db_init(), event);

    INFO("Initialize CC2540");
    hci_device_init(hci);
}

void loop() {
    hci_update(hci);
}

void event(const Event *event) {
    Device *device;
    switch (event->type) {
        case HCI_EVENT_DEVICE_INIT_DONE:
            INFO("CC250 initialized.");
            INFO("Start discovery...");
            digitalWrite(PIN_LED_ON, HIGH);
            hci_start_discovery(hci);
            break;
        case HCI_EVENT_DEVICE_INFORMATION:
            device = (Device *) event->data;
            INFO_DATA("Store device ", &device->addr, sizeof(device->addr));
            hci_device_store(hci, device);
            break;
        case HCI_EVENT_DEVICE_DISCOVERY_DONE:
            INFO("Discovery complete.");
            INFO("Memory: %d", mem_available());
            INFO("Start discovery...");
            hci_start_discovery(hci);
            break;
    }
}
