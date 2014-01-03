#include <Arduino.h>
#include <AltSoftSerial.h>

#include "ble.h"
#include "debug.h"
#include "hci.h"
#include "pin.h"

#define BAUD_RATE 57600

AltSoftSerial BLEMini;

BLE *ble;

void setup() {
    log_init(&Serial, BAUD_RATE);

    log_writeln(F("Configure pins..."));
    pinMode(PIN_LED_WHITE, OUTPUT);
    pinMode(PIN_LED_YELLOW, OUTPUT);
    pinMode(PIN_LED_RED, OUTPUT);

    log_writeln(F("Initializing BLE..."));
    ble = ble_init(&BLEMini, BAUD_RATE);

    log_info(F("Initializing HCI..."));

    log_info(F("Ready."));
    digitalWrite(PIN_LED_WHITE, HIGH);
}

void loop() {
    if (ble_available(ble)) {
        digitalWrite(PIN_LED_YELLOW, HIGH);
    }
}
