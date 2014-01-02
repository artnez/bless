#include <Arduino.h>
#include <avr/pgmspace.h>
#include <stdint.h>
#include <stdarg.h>

#include "debug.h"

#define LOG_BUFSIZE 64

#define LOG_DEBUG 1 << 0
#define LOG_INFO  1 << 1
#define LOG_WARN  1 << 2
#define LOG_ERR   1 << 2

Logger *logger;

Logger *log_init(HardwareSerial *serial, uint32_t baud) {
    logger = (Logger *) malloc(sizeof(Logger));
    logger->serial = serial;
    logger->serial->begin(baud);
}

void log_write(const char *str) {
    logger->serial->println(str);
}

void log_vwrite(const __FlashStringHelper *format, ...) {
    char buf[LOG_BUFSIZE];
    va_list ap;
    va_start(ap, format);
    vsnprintf_P(buf, sizeof(buf), (const char *) format, ap);
    va_end(ap);
    log_write(buf);
}

/**
 * Returns the amount of bytes remaining on the heap. Does not take
 * fragmentation into account.
 *
 * The AVR memory layout places the stack at the end of RAM, after the heap.
 * See: http://www.nongnu.org/avr-libc/user-manual/malloc.html
 *
 * This function is not portable.
 */
int mem_available() {
    extern int __heap_start, *__brkval;
    int stack_pos, heap_pos;
    int v;

    stack_pos = (int) &v;
    heap_pos = (int) (__brkval == 0 ? &__heap_start : __brkval);
    return stack_pos - heap_pos;
}
