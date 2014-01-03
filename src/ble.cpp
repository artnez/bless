#include <AltSoftSerial.h>
#include <stdint.h>

#include "ble.h"

BLE *ble_init(AltSoftSerial *serial, uint32_t baud) {
    BLE *ble = (BLE *) malloc(sizeof(BLE));
    ble->serial = serial;
    ble->serial->begin(baud);
    return ble;
}

int ble_available(BLE *ble) {
    return ble->serial->available();
}

int ble_read(BLE *ble) {
    return ble->serial->read();
}

void ble_write(BLE *ble, uint8_t byte) {
    ble->serial->write(byte);
}
