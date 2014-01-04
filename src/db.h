#ifndef __BLESS_DB_H
#define __BLESS_DB_H

#include <stdint.h>

#include "db.h"

#define DB_MAX_RECORDS 100
#define DB_RECORD_SIZE 6

typedef struct {
    uint8_t data[DB_RECORD_SIZE];
} DBRecord;

typedef struct {
    DBRecord entries[DB_MAX_RECORDS];
    uint8_t len;
} DB;

DB *db_init();
int db_store(DB *db, void *data, size_t size);

#endif
