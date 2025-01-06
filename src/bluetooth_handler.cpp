#include <bluefruit.h>
#include "bluetooth_handler.h"
#include "midi_handler.h"
#include "device_info.h"
#include "lights_handler.h"

BLEDis bledis;

void setupBluetooth() {
    // Config the peripheral connection with maximum bandwidth 
    // more SRAM required by SoftDevice
    // Note: All config***() function must be called before begin()
    Bluefruit.configPrphBandwidth(BANDWIDTH_MAX);  

    Bluefruit.begin();
    Bluefruit.setTxPower(4);    // Check bluefruit.h for supported values

    // Setup the on board blue LED to be disabled on CONNECT
    Bluefruit.autoConnLed(false);

    // configure bt info
    Bluefruit.setName(DEVICE_NAME);
    Bluefruit.setAppearance(BLE_APPEARANCE_GENERIC_EYE_GLASSES);

    // Configure and Start Device Information Service
    bledis.setManufacturer(MANUFACTURER);
    bledis.setModel(MODEL);
    bledis.begin();

    // Register connection and disconnection callbacks
    Bluefruit.Periph.setConnectCallback(connectedCallback);
    Bluefruit.Periph.setDisconnectCallback(disconnectedCallback);
}

void startBTAdvertisement() {
    // Set General Discoverable Mode flag
    Bluefruit.Advertising.addFlags(BLE_GAP_ADV_FLAGS_LE_ONLY_GENERAL_DISC_MODE);

    // Advertise TX Power
    Bluefruit.Advertising.addTxPower();

    // Advertise BLE MIDI Service
    Bluefruit.Advertising.addService(blemidi);

    // Secondary Scan Response packet (optional)
    // Since there is no room for 'Name' in Advertising packet
    Bluefruit.ScanResponse.addName();

    /* Start Advertising
    * - Enable auto advertising if disconnected
    * - Interval:  fast mode = 20 ms, slow mode = 152.5 ms
    * - Timeout for fast mode is 30 seconds
    * - Start(timeout) with timeout = 0 will advertise forever (until connected)
    *
    * For recommended advertising interval
    * https://developer.apple.com/library/content/qa/qa1931/_index.html   
    */
        Bluefruit.Advertising.restartOnDisconnect(true);
    Bluefruit.Advertising.setInterval(32, 244);    // in unit of 0.625 ms
    Bluefruit.Advertising.setFastTimeout(30);      // number of seconds in fast mode
    Bluefruit.Advertising.start(0);                // 0 = Don't stop advertising after n seconds  

    Serial.println("waiting for bluetooth connection...");
}

void connectedCallback(uint16_t conn_handle) {
    Serial.println("bluetooth connected!");
    flashConnected();
}

void disconnectedCallback(uint16_t conn_handle, uint8_t reason) {
    Serial.println("bluetooth disconnected! waiting for connection...");
    flashDisconnected();
}