/**
 * @file control_handler.cpp
 * @brief This file contains the implementation of the control handler for the horse-controller project.
 * 
 * The file provides functions to update the rotary encoder position and set a new encoder value.
 * 
 * Dependencies:
 * - Encoder library
 * 
 * Functions:
 * - void updateEncoder(): Updates the encoder position and prints the new position if it has changed.
 * - void setEncoder(int val): Sets the encoder to a specified value and updates the internal position tracking.
 * 
 * Global Variables:
 * - Encoder mainEncoder: An instance of the Encoder class, initialized with pins 12 and 13.
 * - int encoderPosition: Tracks the current position of the encoder.
 */
#include "control_handler.h"
#include "MIDIButton.h"

// define nice name for button
#define BUTTON_BUILTIN 4

#define ENCODER_PIN_A 18
#define ENCODER_PIN_B 19
#define ENCODER_DEBOUNCE_MS 30

#define PINKY_FINGER_PIN 11
#define RING_FINGER_PIN 10
#define MIDDLE_FINGER_PIN 9
#define INDEX_FINGER_PIN 7

volatile int encoderPosition = 0;
bool lastStateA;
unsigned long timeOfLastChange = 0;

std::vector<MIDIButton> buttons = {
    MIDIButton(PINKY_FINGER_PIN, 19, 1, MIDI_BUTTON_TYPE_CC, "pinky"),
    MIDIButton(RING_FINGER_PIN, 18, 1, MIDI_BUTTON_TYPE_DIAL, "ring"),
    MIDIButton(MIDDLE_FINGER_PIN, 17, 1, MIDI_BUTTON_TYPE_CC, "middle"),
    MIDIButton(INDEX_FINGER_PIN, 16, 1, MIDI_BUTTON_TYPE_CC, "index"),
    MIDIButton(BUTTON_BUILTIN, 60, 1, MIDI_BUTTON_TYPE_NOTE, "builtin")
  };


void encoderISR() {
    int currentStateA = digitalRead(ENCODER_PIN_A);

    if (currentStateA != lastStateA) {
        if (millis() - timeOfLastChange > ENCODER_DEBOUNCE_MS) {
            timeOfLastChange = millis();

            if (digitalRead(ENCODER_PIN_B) != currentStateA) {
                encoderPosition++;
            } else {
                encoderPosition--;
            }
        }
    }
    lastStateA = currentStateA;

}

void setupEncoder() {
    pinMode(ENCODER_PIN_A, INPUT_PULLUP);
    pinMode(ENCODER_PIN_B, INPUT_PULLUP);

    lastStateA = digitalRead(ENCODER_PIN_A);
    
    // Attach interrupt to pin A
    attachInterrupt(digitalPinToInterrupt(ENCODER_PIN_A), encoderISR, CHANGE);
  }

void setEncoder(int val) {
    encoderPosition = val;
}

// TODO: use hardware interrupts instead of this?
void updateControls() {
    for (size_t i = 0; i < buttons.size(); ++i) {
        buttons[i].update();
    }
}