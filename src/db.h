#ifndef __BLESS_DB_H
#define __BLESS_DB_H

#include <stdint.h>

#include "db.h"
#include "hci.h"

#define DB_MAX_ITEMS 32

typedef struct {
    uint8_t active;
    uint16_t time;
    uint8_t data[7];
} Datum;

typedef struct {
    uint8_t index;
    Datum data[DB_MAX_ITEMS];
} DB;

DB *db_init();
int db_save(DB *db, const Device *device);
int db_cmp(const uint8_t *a, const uint8_t *b, size_t s);
int db_size(const DB *db);
void db_log(const DB *db);

#endif
