#include <Arduino.h>
#include <stdlib.h>
#include <string.h>

#include "db.h"

DB *db_init() {
    DB *db = (DB *) malloc(sizeof(DB));
    db->len = 0;
    return db;
}

int db_store(DB *db, const void *data, size_t size) {
    if (db->len >= DB_MAX_RECORDS) {
        return 0;
    }
    memcpy(&db->entries[db->len], data, size);
    db->len ++;
    return 1;
}
