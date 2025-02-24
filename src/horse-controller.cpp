/*********************************************************************
 * horse-controller.cpp - Main file for the Horse Controller
 * 
 * This file is the main file for the Horse Controller. It sets up the
 * Bluetooth connection, MIDI connection, and the button and LED
 * hardware. It also contains the main loop for the controller.
 * 
 * Author: oddhorse (John Trinh)
*********************************************************************/

#include <Arduino.h>
#include <Adafruit_DotStar.h>
#include <bluefruit.h>
#include <Adafruit_TinyUSB.h>
#include <vector>

#include "MIDIButton.h"

#include "display_handler.h"
#include "midi_handler.h"
#include "lights_handler.h"
#include "bluetooth_handler.h"
#include "device_info.h"
#include "serial_info.h"


// define nice name for button
#define BUTTON_BUILTIN 4

std::vector<MIDIButton> buttons = {
  MIDIButton(7, 16, 1, MIDI_BUTTON_TYPE_CC),
  MIDIButton(9, 17, 1, MIDI_BUTTON_TYPE_CC),
  MIDIButton(10, 18, 1, MIDI_BUTTON_TYPE_CC),
  MIDIButton(11, 19, 1, MIDI_BUTTON_TYPE_CC),
  MIDIButton(BUTTON_BUILTIN, 60, 1, MIDI_BUTTON_TYPE_NOTE)
};

void setup()
{

  Serial.begin(115200);

  setupDisplay();
  setupBluetooth();
  setupMidi(); //blemidi called in here
  startBTAdvertisement();
  setupLights();
}


void loop() {
  updateSerial();

  updateLED();

  // Don't continue if we aren't connected or the connected device isn't ready to receive messages.
  if (! midiReady()) {
    return;
  }

  // Iterate over all buttons in the vector
  for (size_t i = 0; i < buttons.size(); ++i) {
    buttons[i].update();
  }

  
  updateDotstar();
}
