#include "serial_info.h"
#include "midi_handler.h"
#include "device_info.h"

bool splashPrinted = false;
bool midiReadyTimePrinted = false;
int midiReadyTime = -1;
bool BTConnectedTimePrinted = false;
int BTConnectedTime = -1;
unsigned long lastDisconnect = 0;

void updateSerial() {
    if (Serial && !splashPrinted) {
        Serial.printf("%s model %s\n", DEVICE_NAME, MODEL);
        Serial.printf("firmware ver. %s\n", FIRMWARE_VERSION);
        Serial.printf("by %s\n", MANUFACTURER);
        Serial.printf("initialized serial in %d ms\n", millis());

        splashPrinted = true;
    }

    if (Bluefruit.connected() && BTConnectedTime == -1) {
        BTConnectedTime = millis() - lastDisconnect;
    } else if (!Bluefruit.connected() && BTConnectedTime != -1) {
        lastDisconnect = millis();
        BTConnectedTime = -1;
        BTConnectedTimePrinted = false;
        midiReadyTime = -1;
        midiReadyTimePrinted = false;
    }

    if (Serial && (BTConnectedTime != -1) && !BTConnectedTimePrinted) {
        Serial.printf("bluetooth connected in %d ms\n", BTConnectedTime);
        BTConnectedTimePrinted = true;
    }

    if (midiReady() && midiReadyTime == -1) {
        midiReadyTime = millis() - lastDisconnect;
    }

    if (Serial && (midiReadyTime != -1) && !midiReadyTimePrinted) {
        Serial.printf("midi ready in %d ms\n", midiReadyTime);
        midiReadyTimePrinted = true;
    }
}
