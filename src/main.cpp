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

void initialize();
void discover();
void timeout();
void event(const Event *event);
void blink_active_led();

AltSoftSerial BLEMini;
BLE *ble;
HCI *hci;
Timer *timer;

void setup() {
    log_init(&Serial, BAUD_RATE);
    INFO("Starting up...");

    pinMode(PIN_LED_ACTIVE, OUTPUT);
    pinMode(PIN_LED_ERROR, OUTPUT);
    pinMode(PIN_LED_RX, OUTPUT);
    pinMode(PIN_LED_TX, OUTPUT);

    ble = ble_init(&BLEMini, BAUD_RATE);
    hci = hci_init(ble, db_init(), event);
    timer = timer_init(timeout);

    initialize();
}

void loop() {
    blink_active_led();
    hci_update(hci);
    timer_update(timer);
}

void initialize() {
    INFO("Initialize CC2540...");
    delay(1000);
    timer_set(timer, STATE_INITIALZING, TIMEOUT_INITIALIZE);
    hci_device_init(hci);
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
        Device *device = (Device *) event->data;
        DUMP("Device Address: ", &device->addr, sizeof(device->addr));
        INFO("Device Response: %#04x", device->event_type);
        INFO("Device RSSI: %d", device->rssi);
        //INFO("Device Stored: %d", store(device));
        return;
    }

    if (event->type == HCI_EVENT_DEVICE_DISCOVERY_DONE) {
        digitalWrite(PIN_LED_ERROR, LOW);
        INFO("Discovery complete.");
        INFO("Memory: %d", mem_available());
        INFO("Cycles: %d", hci->cycles);
        INFO("Events: %d", hci->events);
        INFO("Running time: %ld", millis());
        INFO("");
        discover();
        return;
    }

    INFO("Unhandled event %#04x, %#06x", event->status, event->type);
}

void timeout() {
    INFO("State %d timeout in %ldms. Reset...", timer->code, timer->timeout);
    timer_set(timer, timer->code, timer->timeout);
    switch (timer->code) {
        case STATE_INITIALZING:
            initialize();
            break;
        case STATE_DISCOVERING:
            digitalWrite(PIN_LED_ERROR, HIGH);
            discover();
            break;
        default:
            INFO("Rebooting...");
            delay(1000);
            asm volatile ("jmp 0");
    }
}

void blink_active_led() {
    static unsigned long timer = 0;
    if (millis() - timer < 100) return;
    timer = millis();
    digitalWrite(PIN_LED_ACTIVE, digitalRead(PIN_LED_ACTIVE) ? LOW : HIGH);
}
