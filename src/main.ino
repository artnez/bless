#include <AltSoftSerial.h>

#include "ble.h"
#include "debug.h"
#include "hci.h"
#include "pin.h"

#define BAUD_RATE 57600

AltSoftSerial Device;

BLE *ble;

void setup() {
    log_init(&Serial, BAUD_RATE);

    log_info(F("Configure pins..."));
    pinMode(PIN_LED_WHITE, OUTPUT);
    pinMode(PIN_LED_YELLOW, OUTPUT);
    pinMode(PIN_LED_RED, OUTPUT);

    log_info(F("Initializing BLE..."));
    ble = ble_init(&Device, BAUD_RATE);

    log_info(F("Ready."));
    digitalWrite(PIN_LED_WHITE, HIGH);

    log_info(F("%d"), mem_available());
}

void loop() {
    if (ble_available(ble)) {
        digitalWrite(PIN_LED_YELLOW, HIGH);
    }
}
