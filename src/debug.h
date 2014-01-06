#ifndef __BLESS_DEBUG_H
#define __BLESS_DEBUG_H

#include <Arduino.h>
#include <stdint.h>

// Workaround for http://gcc.gnu.org/bugzilla/show_bug.cgi?id=34734
#ifdef PROGMEM
#undef PROGMEM
#define PROGMEM __attribute__((section(".progmem.data")))
#endif

#define INFO(fmt, ...) log_writeln(F(fmt), ##__VA_ARGS__);
#define WARN(fmt, ...) \
    log_writeln(F("[%s:%d] " fmt), __FUNCTION__, __LINE__, ##__VA_ARGS__);
#define DUMP(fmt, ptr, size, ...) \
    log_write(F(fmt), ##__VA_ARGS__); \
    log_dump(ptr, size); \
    log_writeln(F(""));

#ifdef BLESS_DEBUG
    #define DEBUG(fmt, ...) INFO(fmt, ##__VA_ARGS__);
    #define DEBUG_DUMP(fmt, ptr, size, ...) DUMP(fmt, ptr, size, ##__VA_ARGS__);
#else
    #define DEBUG(fmt, ...)
    #define DEBUG_DUMP(fmt, ptr, size, ...)
#endif

#define ALLOC(N, T, S) \
    T *N = (T *) malloc(S); \
    if (!N) { \
        WARN("Could not allocate #T (%ld) in %s:%s", S, __FILE__, __LINE__); \
    }

#define ALLOC_STRUCT(N, T) ALLOC(N, T, sizeof(T));

typedef struct {
    HardwareSerial *serial;
} Logger;

Logger *log_init(HardwareSerial *serial, uint32_t baud);
void log_write(const __FlashStringHelper *format, ...);
void log_writeln(const __FlashStringHelper *format, ...);
void log_dump(const void *ptr, uint8_t size);

int mem_available();

#endif
