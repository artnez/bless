#include <Arduino.h>
#include <stdlib.h>
#include <string.h>

#include "db.h"
#include "debug.h"

DB *db_init() {
    ALLOC_STRUCT(db, DB);
    if (!db) return NULL;
    return db;
}
