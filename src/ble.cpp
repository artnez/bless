#include <AltSoftSerial.h>
#include <stdint.h>

#include "ble.h"
#include "debug.h"
#include "pin.h"

#define BLE_IO_DELAY 20

BLE *ble_init(AltSoftSerial *serial, uint32_t baud) {
    BLE *ble = (BLE *) malloc(sizeof(BLE));
    ble->serial = serial;
    ble->serial->begin(baud);
    return ble;
}

int ble_available(const BLE *ble) {
    return ble->serial->available();
}

int ble_read(const BLE *ble) {
    digitalWrite(PIN_LED_RX, HIGH);
    delay(BLE_IO_DELAY);
    int data = ble->serial->read();
    digitalWrite(PIN_LED_RX, LOW);
    return data;
}

void ble_write(const BLE *ble, const uint8_t *bytes, size_t len) {
    digitalWrite(PIN_LED_TX, HIGH);
    delay(BLE_IO_DELAY);
    ble->serial->write(bytes, len);
    digitalWrite(PIN_LED_TX, LOW);
}

void ble_log_write(const uint8_t *bytes, size_t len) {
    if (len < 1) {
        return;
    }
    log_writeln(F(""));
    log_writeln(F("> write"));
    for (int i=0; i<len; i++) {
        if (i % 8 == 0) {
            if (i) log_writeln(F(""));
            log_write(F("> "));
        }
        log_write(F("%02x "), bytes[i]);
    }
    log_writeln(F(""));
    log_writeln(F(""));
}
