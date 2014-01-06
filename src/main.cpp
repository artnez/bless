#include <Arduino.h>
#include <AltSoftSerial.h>
#include <string.h>

#include "ble.h"
#include "db.h"
#include "debug.h"
#include "hci.h"
#include "pin.h"
#include "timer.h"

#define BAUD_RATE 57600

#define TIMEOUT_INITIALIZE 5000
#define TIMEOUT_DISCOVER   30000

#define STATE_INITIALZING 1 << 0
#define STATE_DISCOVERING 1 << 1

void discover();
void timeout();
void event(const Event *event);
void blink_active_led();

AltSoftSerial BLEMini;

BLE *ble;
DB *db;
HCI *hci;
Timer *timer;

void setup() {
    log_init(&Serial, BAUD_RATE);
    INFO("Starting up...");

    pinMode(PIN_LED_ACTIVE, OUTPUT);
    pinMode(PIN_LED_TIMEOUT, OUTPUT);
    pinMode(PIN_LED_RX, OUTPUT);
    pinMode(PIN_LED_TX, OUTPUT);

    ble = ble_init(&BLEMini, BAUD_RATE);
    db = db_init();
    hci = hci_init(ble, event);
    timer = timer_init(timeout);

    INFO("Initialize CC2540...");
    delay(1000);
    timer_set(timer, STATE_INITIALZING, TIMEOUT_INITIALIZE);
    hci_device_init(hci);
}

void loop() {
    blink_active_led();
    hci_update(hci);
    timer_update(timer);
}

void discover() {
    delay(1000);
    INFO("Start discovery...");
    timer_set(timer, STATE_DISCOVERING, TIMEOUT_DISCOVER);
    hci_start_discovery(hci);
}

void event(const Event *event) {
    if (event->type == HCI_EVENT_DEVICE_INIT_DONE) {
        INFO("CC250 initialized.");
        discover();
        return;
    }

    if (event->type == HCI_EVENT_DEVICE_INFORMATION) {
        Device *device = device_init(event->data);
        DUMP("Device Addr: ", &device->addr, sizeof(device->addr));
        INFO("Device RSSI: %d", device->rssi);
        INFO("Saved Index: %d", db_save(db, device));
        device_free(device);
        return;
    }

    if (event->type == HCI_EVENT_DEVICE_DISCOVERY_DONE) {
        hci->cycles ++;
        digitalWrite(PIN_LED_TIMEOUT, LOW);
        INFO("Discovery complete.");
        INFO("Memory: %d", mem_available());
        INFO("Runtime: %ld", millis());
        INFO("HCI Cycles: %d", hci->cycles);
        INFO("HCI Events: %d", hci->events);
        INFO("");
        INFO("%d Known Devices", db_size(db));
        db_log(db);
        INFO("");
        discover();
        return;
    }

    INFO("Unhandled event %#04x, %#06x", event->status, event->type);
}

void timeout() {
    digitalWrite(PIN_LED_TIMEOUT, HIGH);
    INFO("State %d timeout in %ldms. Reset...", timer->code, timer->timeout);
    delay(1000);

    // XXX: This isn't a true reset. Use the watchdog timer:
    // http://www.nongnu.org/avr-libc/user-manual/group__avr__watchdog.html
    asm volatile ("jmp 0");
}

void blink_active_led() {
    static unsigned long timer = 0;
    if (millis() - timer < 100) return;
    timer = millis();
    digitalWrite(PIN_LED_ACTIVE, digitalRead(PIN_LED_ACTIVE) ? LOW : HIGH);
}
