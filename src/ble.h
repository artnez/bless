#ifndef __BLESS_BLE_H
#define __BLESS_BLE_H

#include <AltSoftSerial.h>
#include <stdint.h>

typedef struct {
    AltSoftSerial *serial;
} BLE;

BLE *ble_init(AltSoftSerial *serial, uint32_t baud);
int ble_available(BLE *ble);
int ble_read(BLE *ble);
void ble_write(BLE *ble, uint8_t byte);

#endif
