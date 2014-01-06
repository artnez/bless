#include <Arduino.h>
#include <stdlib.h>
#include <string.h>

#include "db.h"
#include "debug.h"
#include "hci.h"

// XXX: Clean up this whole thing. Needs to be generic. Shouldn't need hci.h

DB *db_init() {
    DB *db = (DB *) calloc(1, sizeof(db->data));
    if (!db) return NULL;
    db->index = 0;
    return db;
}

int db_save(DB *db, const Device *device) {
    uint8_t index = db->index;
    uint8_t addr_size = sizeof(device->addr);

    Datum *datum;
    for (int i=0; i<DB_MAX_ITEMS; i++) {
        datum = &db->data[i];
        if (!datum->active) continue;

        Device *record = (Device *) datum->data;
        if (db_cmp(record->addr, device->addr, sizeof(device->addr))) {
            memset(&datum->time, 0, sizeof(datum->time));
            record->rssi = device->rssi;
            return i;
        }
    }

    datum = (Datum *) &(db->data[index]);
    datum->active = 1;
    memset(&datum->time, 0, sizeof(datum->time));
    memcpy(&datum->data, device, sizeof(datum->data));

    if ((++ db->index) >= DB_MAX_ITEMS) {
        db->index = 0;
    }
    return index;
}

void db_clear(DB *db) {
    db->index = 0;
    memset(db->data, 0, sizeof(db->data));
}

int db_cmp(const uint8_t *a, const uint8_t *b, size_t s) {
    for (int i=0; i<s; i++) {
        if (a[i] != b[i]) {
            return 0;
        }
    }
    return 1;
}

int db_size(const DB *db) {
    uint8_t count = 0;
    for (int i=0; i<DB_MAX_ITEMS; i++) {
        const Datum *datum = &db->data[i];
        if (!datum->active) continue;
        count ++;
    }
    return count;
}

void db_log(const DB *db) {
    for (int i=0; i<DB_MAX_ITEMS; i++) {
        const Datum *datum = &db->data[i];
        if (!datum->active) continue;

        Device *device = (Device *) &datum->data;
        log_write(F("[time %d] [rssi %d] "), datum->time, device->rssi);
        for (int j=0, m=sizeof(device->addr); j<m; j++) {
            log_write(F(" %02x"), device->addr[j]);
        }
        log_writeln(F(""));
    }
}
