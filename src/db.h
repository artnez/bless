#ifndef __BLESS_DB_H
#define __BLESS_DB_H

#include <stdint.h>

#include "db.h"

#define DB_MAX_ITEMS 100
#define DB_ITEM_SIZE 6

typedef struct {
    uint8_t data[DB_ITEM_SIZE];
} DBItem;

typedef struct {
    DBItem items[DB_MAX_ITEMS];
} DB;

DB *db_init();

#endif
