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
    log_writeln(F("Starting up..."));

    pinMode(PIN_LED_ON, OUTPUT);
    pinMode(PIN_LED_RX, OUTPUT);
    pinMode(PIN_LED_TX, OUTPUT);

    ble = ble_init(&BLEMini, BAUD_RATE);
    hci = hci_init(ble, db_init(), event);

    log_writeln(F("Initialize CC2540..."));
    hci_device_init(hci);
}

void loop() {
    hci_update(hci);
}

void event(const Event *event) {
    Device *device;
    switch (event->type) {
        case HCI_EVENT_DEVICE_INIT_DONE:
            log_writeln(F("CC250 initialized."));
            log_writeln(F("Start discovery..."));
            digitalWrite(PIN_LED_ON, HIGH);
            hci_start_discovery(hci);
            break;
        case HCI_EVENT_DEVICE_INFORMATION:
            device = (Device *) event->data;
            log_write(F("Store device "));
            hci_log_data(&device->addr, sizeof(device->addr));
            if (hci_device_store(hci, device)) {
                log_writeln(F("(OK)"));
            } else {
                log_writeln(F("(FAIL)"));
            }
            break;
        case HCI_EVENT_DEVICE_DISCOVERY_DONE:
            log_writeln(F("Discovery complete."));
            log_writeln(F("Memory: %d"), mem_available());
            log_writeln(F("Start discovery..."));
            hci_start_discovery(hci);
            break;
    }
}
