#include <Arduino.h>
#include <AltSoftSerial.h>

#include "ble.h"
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

    pinMode(PIN_LED_WHITE, OUTPUT);
    pinMode(PIN_LED_YELLOW, OUTPUT);
    pinMode(PIN_LED_RED, OUTPUT);

    ble = ble_init(&BLEMini, BAUD_RATE);
    hci = hci_init(ble, event);
    hci_device_init(hci);
}

void loop() {
    hci_update(hci);
}

void event(const Event *event) {
    if (hci_event(event, HCI_EVENT_COMMAND_STATUS, HCI_OPCODE_INIT_DEVICE)) {
        digitalWrite(PIN_LED_WHITE, HIGH);
        return;
    }

    status();
}

void status() {
    log_writeln(F("* memory available: %d"), mem_available());
}
