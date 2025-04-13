/**
 * @file control_handler.cpp
 * @brief implementation for handling MIDI controller inputs (buttons and encoder).
 * @author oddhorse (John Trinh)
 * @copyright copyright (c) 2025 oddhorse
 *
 * manages input from multiple MIDI buttons and rotary encoder.
 * handles button state tracking, debouncing, and MIDI signal generation.
 */

// TODO: doc this file, verify header comment is correct

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
#define INDEX_FINGER_PIN 12

namespace
{
	bool lastStateA;

	void encoderISR()
	{
		int currentStateA = digitalRead(ENCODER_PIN_A);

		if (currentStateA != lastStateA)
		{
			if (millis() - Ctrls::timeOfLastChange > ENCODER_DEBOUNCE_MS)
			{
				Ctrls::timeOfLastChange = millis();

				if (digitalRead(ENCODER_PIN_B) != currentStateA)
				{
					Ctrls::encoderPosition++;
				}
				else
				{
					Ctrls::encoderPosition--;
				}
			}
		}
		lastStateA = currentStateA;
	}
}

namespace Ctrls
{
	volatile int encoderPosition = 0;
	unsigned long timeOfLastChange = 0;

	std::vector<MIDIButton> buttons = {
		MIDIButton(PINKY_FINGER_PIN, 49, 1, MIDI_BUTTON_TYPE_PUNCH, "pinky"),
		MIDIButton(RING_FINGER_PIN, 42, 1, MIDI_BUTTON_TYPE_DIAL, "ring"),
		// MIDIButton(MIDDLE_FINGER_PIN, 17, 1, MIDI_BUTTON_TYPE_CC, "middle"),
		// MIDIButton(INDEX_FINGER_PIN, 16, 1, MIDI_BUTTON_TYPE_CC, "index"),
		MIDIButton(MIDDLE_FINGER_PIN, 38, 1, MIDI_BUTTON_TYPE_CC, "middle"),
		MIDIButton(INDEX_FINGER_PIN, 36, 1, MIDI_BUTTON_TYPE_PUNCH, "index"),
		MIDIButton(BUTTON_BUILTIN, 60, 1, MIDI_BUTTON_TYPE_NOTE, "builtin")};

	void setupEncoder()
	{
		pinMode(ENCODER_PIN_A, INPUT_PULLUP);
		pinMode(ENCODER_PIN_B, INPUT_PULLUP);

		lastStateA = digitalRead(ENCODER_PIN_A);

		// Attach interrupt to pin A
		attachInterrupt(digitalPinToInterrupt(ENCODER_PIN_A), encoderISR, CHANGE);
	}

	void setEncoder(int val)
	{
		encoderPosition = val;
	}

	// TODO: use hardware interrupts instead of this?

	void updateControls()
	{
		for (size_t i = 0; i < buttons.size(); ++i)
		{
			buttons[i].update();
		}
	}
}