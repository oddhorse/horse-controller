// TODO: doc this file
#ifndef BLUETOOTH_HANDLER_H
#define BLUETOOTH_HANDLER_H

#include <Arduino.h>
#include <bluefruit.h>

namespace BT
{
    void setupBluetooth();
    void startBTAdvertisement();
}
#endif // BLUETOOTH_HANDLER_H