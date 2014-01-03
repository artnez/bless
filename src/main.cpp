#include <Arduino.h>
#include <AltSoftSerial.h>

#include "ble.h"
#include "debug.h"
#include "hci.h"
#include "pin.h"

#define BAUD_RATE 57600

void event(const Event *event);

AltSoftSerial BLEMini;
BLE *ble;
HCI *hci;

void setup() {
    log_init(&Serial, BAUD_RATE);

    log_writeln(F("Configure pins..."));
    pinMode(PIN_LED_WHITE, OUTPUT);
    pinMode(PIN_LED_YELLOW, OUTPUT);
    pinMode(PIN_LED_RED, OUTPUT);

    log_writeln(F("Initializing BLE..."));
    ble = ble_init(&BLEMini, BAUD_RATE);

    log_writeln(F("Initializing HCI..."));
    hci = hci_init(ble, event);
    hci_device_init(hci);
}

void loop() {
    hci_update(hci);
}

void event(const Event *event) {}
