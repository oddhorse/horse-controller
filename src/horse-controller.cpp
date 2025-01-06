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

#include "midi_handler.h"
#include "lights_handler.h"
#include "bluetooth_handler.h"
#include "device_info.h"
#include "serial_info.h"

// define nice name for button
#define BUTTON_BUILTIN 4

void setup()
{

  Serial.begin(115200);


  setupBluetooth();
  setupMidi(); //blemidi called in here
  
  // Set up and start advertising
  startBTAdvertisement();
  
  // button and led setup
  pinMode(BUTTON_BUILTIN, INPUT_PULLUP);

  setupLights();

  // Start MIDI read loop
  Scheduler.startLoop(midiRead);
}

bool buttonState = false;
bool buttonStateLast = false;

void loop() {
  updateSerial();

  updateLED();
  // STATE READS
  // builtin button
  buttonState = (digitalRead(BUTTON_BUILTIN) == LOW) ? true : false;

  // Don't continue if we aren't connected or the connected device isn't ready to receive messages.
  if (! midiReady()) {
    return;
  }

  if (buttonState == true && buttonStateLast == false) {
    MIDI.sendNoteOn(60, 127, 1);
    updateLastButtonPress();
  } else if (buttonState == false && buttonStateLast == true) {
    MIDI.sendNoteOff(60, 0, 1);
  }
  updateDotstar();

  // SAVE STATE
  buttonStateLast = buttonState;
}
