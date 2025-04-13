/**
 * @file midibutton.cpp
 * @brief implementation of midi button functionality that responds to button presses and sends midi signals
 *
 * handles different button types:
 * - note buttons: send note on/off messages
 * - cc buttons: send control change messages
 * - dial buttons: send continuous control change values
 *
 * includes debounce logic for reliable button state detection
 *
 * @author oddhorse (John Trinh)
 * @copyright copyright (c) 2025 oddhorse
 */
#include "MIDIButton.h"
#include "lights_handler.h"
#include "display_handler.h"
#include "imu_handler.h"

// TODO: make new parameter noteValue that is separate from noteValue to store midi note of button
// TODO: rename params dataValue, and currentValue to less confusing things

MIDIButton::MIDIButton(uint8_t pin, byte dataValue, byte channel, byte buttonType, const char buttonName[])
	: name(buttonName), currentValue(0), buttonType(buttonType), _pin(pin), _dataValue(dataValue), _channel(channel), _currentState(HIGH), _lastState(HIGH), _debouncer(Bounce()), _isNoteOn(false), _noteTime(millis())
{
	pinMode(_pin, INPUT_PULLUP);
	_debouncer.attach(_pin);
	_debouncer.interval(5); // TODO: parameterize this?
}

void MIDIButton::update()
{
	_debouncer.update();

	_currentState = _debouncer.read();

	if (buttonType == MIDI_BUTTON_TYPE_PUNCH && _isNoteOn)
	{
		MIDI.sendNoteOff(_dataValue, 127, _channel);
		_isNoteOn = false;
	}

	if (buttonType == MIDI_BUTTON_TYPE_PUNCH && isPressed())
	{
		if (noteTriggered)
		{
			Serial.print("Note On: ");
			Serial.println(_dataValue);
			MIDI.sendNoteOn(_dataValue, 127, _channel);
			_isNoteOn = true;
			noteTriggered = false;
		}
	}

	if (buttonType == MIDI_BUTTON_TYPE_DIAL && isPressed())
	{
		MIDI.sendControlChange(_dataValue, getYAccelVal(), _channel);
	}

	if (buttonType != MIDI_BUTTON_TYPE_PUNCH)
	{
		if (isRisingEdge())
		{ // Button Pressed
			Serial.print("Note On: ");
			Serial.println(_dataValue);
			currentValue = 127;
			if (buttonType == MIDI_BUTTON_TYPE_NOTE)
			{
				MIDI.sendNoteOn(_dataValue, 127, _channel);
			}
			else if (buttonType == MIDI_BUTTON_TYPE_CC)
			{
				MIDI.sendControlChange(_dataValue, 127, _channel);
			}
			updateLastButtonPress(); // update dotstar
		}
		else if (isFallingEdge())
		{ // Button Released
			Serial.print("Note Off: ");
			Serial.println(_dataValue);
			currentValue = 0;
			if (buttonType == MIDI_BUTTON_TYPE_NOTE)
			{
				MIDI.sendNoteOff(_dataValue, 0, _channel);
			}
			else if (buttonType == MIDI_BUTTON_TYPE_CC)
			{
				MIDI.sendControlChange(_dataValue, 0, _channel);
			}
		}
	}

	_lastState = _currentState;
}

bool MIDIButton::isPressed()
{
	return _currentState == LOW;
}

bool MIDIButton::isRisingEdge()
{
	return _currentState == LOW && _lastState == HIGH;
}

bool MIDIButton::isFallingEdge()
{
	return _currentState == HIGH && _lastState == LOW;
}