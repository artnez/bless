#ifndef __BLESS_BLE_H
#define __BLESS_BLE_H

#include <AltSoftSerial.h>
#include <stdint.h>

typedef struct {
    AltSoftSerial *serial;
} BLE;

BLE *ble_init(AltSoftSerial *serial, uint32_t baud);
int ble_available(const BLE *ble);
int ble_read(const BLE *ble);
void ble_write(const BLE *ble, const uint8_t *bytes, size_t len);

void ble_log_write(const uint8_t *bytes, size_t len);

#endif
