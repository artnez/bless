#include <Arduino.h>
#include <stdlib.h>
#include <string.h>

#include "db.h"

DB *db_init() {
    DB *db = (DB *) malloc(sizeof(DB));
    return db;
}
