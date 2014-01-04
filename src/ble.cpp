#include <AltSoftSerial.h>
#include <stdint.h>

#include "ble.h"
#include "debug.h"

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
    return ble->serial->read();
}

void ble_write(const BLE *ble, const uint8_t *bytes, size_t len) {
    #if BLESS_DEBUG
    ble_log_write(bytes, len);
    #endif
    ble->serial->write(bytes, len);
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
