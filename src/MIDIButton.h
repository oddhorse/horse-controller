/**
 * @file MIDIButton.h
 * @brief handles button inputs and sending midi commands
 * @author oddhorse (John Trinh)
 * @copyright copyright (c) 2025 oddhorse
 */
#ifndef MIDIBUTTON_H
#define MIDIBUTTON_H

#include <Arduino.h>
#include <MIDI.h>
#include <bluefruit.h>
#include <Bounce2.h>

#define MIDI_BUTTON_TYPE_NOTE 0
#define MIDI_BUTTON_TYPE_CC 1
#define MIDI_BUTTON_TYPE_DIAL 2
#define MIDI_BUTTON_TYPE_PUNCH 3

extern midi::MidiInterface<midi::SerialMIDI<BLEMidi>> MIDI;

class MIDIButton
{
public:
	// TODO: don't forget to change dataValue name in docs!
	/**
	 * @brief creates midi button
	 * @param pin hardware pin for button input
	 * @param dataValue cc control number/midi note of button
	 * @param channel midi channel to use
	 * @param buttonType type of button. may be one of: `MIDI_BUTTON_TYPE_NOTE`, `MIDI_BUTTON_TYPE_CC`, or `MIDI_BUTTON_TYPE_DIAL`
	 * @param buttonName (optional) button name for convenience.
	 */
	MIDIButton(uint8_t pin, byte dataValue, byte channel, byte buttonType, const char buttonName[] = nullptr);

	void update();
	bool isPressed();
	bool isRisingEdge();
	bool isFallingEdge();
	const char *name;
	byte currentValue;
	byte buttonType;

private:
	uint8_t _pin;
	byte _dataValue;
	byte _channel;
	bool _currentState;
	bool _lastState;
	Bounce _debouncer;
	bool _isNoteOn;
};

#endif