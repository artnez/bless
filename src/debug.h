#ifndef __BLESS_DEBUG_H
#define __BLESS_DEBUG_H

#include <Arduino.h>
#include <stdint.h>

// Workaround for http://gcc.gnu.org/bugzilla/show_bug.cgi?id=34734
#ifdef PROGMEM
#undef PROGMEM
#define PROGMEM __attribute__((section(".progmem.data")))
#endif

#ifdef BLESS_DEBUG
#define log_debug(format, ...) log_writeln(format, ##__VA_ARGS__);
#else
#define log_debug(format, ...)
#endif

typedef struct {
    HardwareSerial *serial;
} Logger;

Logger *log_init(HardwareSerial *serial, uint32_t baud);
void log_write(const __FlashStringHelper *format, ...);
void log_writeln(const __FlashStringHelper *format, ...);

int mem_available();

#endif
