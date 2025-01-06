#ifndef BLUETOOTH_HANDLER_H
#define BLUETOOTH_HANDLER_H

#include <Arduino.h>
#include <bluefruit.h>

extern BLEDis bledis;

void setupBluetooth();
void startBTAdvertisement();
void connectedCallback(uint16_t conn_handle);
void disconnectedCallback(uint16_t conn_handle, uint8_t reason);

#endif // BLUETOOTH_HANDLER_H