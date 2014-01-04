#ifndef __BLESS_DEBUG_H
#define __BLESS_DEBUG_H

#include <Arduino.h>
#include <stdint.h>

// Workaround for http://gcc.gnu.org/bugzilla/show_bug.cgi?id=34734
#ifdef PROGMEM
#undef PROGMEM
#define PROGMEM __attribute__((section(".progmem.data")))
#endif

#define INFO(fmt, ...) \
    log_writeln(F(fmt), ##__VA_ARGS__);
#define INFO_PART(fmt, ...) \
    log_write(F(fmt), ##__VA_ARGS__);
#define INFO_DATA(fmt, ptr, size, ...) \
    INFO_PART(fmt, ##__VA_ARGS__); log_data(ptr, size); INFO("");

#ifdef BLESS_DEBUG
    #define DEBUG(fmt, ...) \
        INFO(fmt, ##__VA_ARGS__);
    #define DEBUG_PART(fmt, ...) \
        INFO_PART(fmt, ##__VA_ARGS__);
    #define DEBUG_DATA(fmt, ptr, size, ...) \
        INFO_DATA(fmt, ptr, size, ##__VA_ARGS__);
#else
    #define DEBUG(fmt, ...)
    #define DEBUG_PART(fmt, ...)
    #define DEBUG_DATA(fmt, ptr, size, ...)
#endif

typedef struct {
    HardwareSerial *serial;
} Logger;

Logger *log_init(HardwareSerial *serial, uint32_t baud);
void log_write(const __FlashStringHelper *format, ...);
void log_writeln(const __FlashStringHelper *format, ...);
void log_data(const void *ptr, uint8_t size);

int mem_available();

#endif
