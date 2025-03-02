/**
 * @file horse-controller.cpp
 * @brief entry file for horse controller
 *
 * standard setup/loop arduino construction
 *
 * @author oddhorse (John Trinh)
 */
// TODO: use namespaces in __handler files

// TODO: use constexpr instead of #defines for constants when possible

#include <Arduino.h>
#include <Adafruit_DotStar.h>
#include <bluefruit.h>
#include <Adafruit_TinyUSB.h>

#include "MIDIButton.h"
#include "imu_handler.h"
// #include "display_handler.h"
#include "control_handler.h"
#include "midi_handler.h"
#include "lights_handler.h"
#include "bluetooth_handler.h"
#include "device_info.h"
#include "serial_info.h"

void setup()
{
	Serial.begin(115200);
	// while ( !Serial ) delay(10);

	Serial.println("setting up encoder...");
	Ctrls::setupEncoder();
	// Display::setupDisplay();
	Serial.println("setting up bluetooth...");
	BT::setupBluetooth();
	Serial.println("setting up midi...");
	setupMidi(); // blemidi called in here
	Serial.println("starting bt advertisement...");
	BT::startBTAdvertisement();
	Serial.println("setting up lights...");
	setupLights();
	Serial.println("setting up IMU...");
	setupIMU();
	Serial.println("done setting up!");
	dbgMemInfo();
	Serial.println("waiting 3 seconds");
	delay(3000);
}

void scanI2C()
{
	Serial.println("Scanning I2C...");
	for (byte address = 1; address < 127; address++)
	{
		Wire.beginTransmission(address);
		if (Wire.endTransmission() == 0)
		{
			Serial.print("Found device at 0x");
			Serial.println(address, HEX);
		}
	}
}

void loop()
{
	// scanI2C();
	// if (Serial) Serial.println(millis());
	updateSerial();
	// dbgMemInfo();
	updateLED();

	updateFilter();
	printMadgwick();

	Serial.println(">deviceIsStationary:" + String(deviceIsStationary));

	// Serial.println(getDialValue());

	// if (Serial) printIMUReading();

	// Don't continue if we aren't connected or the connected device isn't ready to receive messages.
	if (!midiReady())
	{
		return;
	}

	Ctrls::updateControls();

	updateDotstar();
}
