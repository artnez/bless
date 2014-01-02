#ifndef __BLESS_DEBUG_H
#define __BLESS_DEBUG_H

#include <Arduino.h>
#include <stdint.h>

// Workaround for http://gcc.gnu.org/bugzilla/show_bug.cgi?id=34734
#ifdef PROGMEM
#undef PROGMEM
#define PROGMEM __attribute__((section(".progmem.data")))
#endif

#ifndef BLESS_DEBUG
#define BLESS_DEBUG 0
#endif

#define log_info(format, ...) log_vwrite(format, ##__VA_ARGS__);
#define log_debug(format, ...) \
    if (BLESS_DEBUG) log_vwrite(format, ##__VA_ARGS__);

typedef struct {
    HardwareSerial *serial;
} Logger;

Logger *log_init(HardwareSerial *serial, uint32_t baud);
void log_write(const char *str);
void log_vwrite(const __FlashStringHelper *format, ...);

int mem_available();

#endif
