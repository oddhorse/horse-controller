#include "MIDIButton.h"
#include "lights_handler.h"
#include "display_handler.h"

MIDIButton::MIDIButton(uint8_t pin, byte dataValue, byte channel, byte buttonType)
  : _pin(pin), _dataValue(dataValue), _channel(channel), _buttonType(buttonType), _currentState(HIGH), _lastState(HIGH), _debouncer(Bounce()) {
  pinMode(_pin, INPUT_PULLUP);
  _debouncer.attach(_pin);
  _debouncer.interval(5); // TODO: parameterize this?
}

void MIDIButton::update() {
    _debouncer.update();

    _currentState = _debouncer.read();

  if (isRisingEdge()) {  // Button Pressed
    Serial.print("Note On: ");
    Serial.println(_dataValue);
    if (_buttonType == MIDI_BUTTON_TYPE_NOTE) {
      MIDI.sendNoteOn(_dataValue, 127, _channel);
    } else if (_buttonType == MIDI_BUTTON_TYPE_CC) {
      MIDI.sendControlChange(_dataValue, 127, _channel);
    }
    updateLastButtonPress();
  } 
  else if (isFallingEdge()) {  // Button Released
    Serial.print("Note Off: ");
    Serial.println(_dataValue);
    if (_buttonType == MIDI_BUTTON_TYPE_NOTE) {
      MIDI.sendNoteOff(_dataValue, 0, _channel);
    } else if (_buttonType == MIDI_BUTTON_TYPE_CC) {
      MIDI.sendControlChange(_dataValue, 0, _channel);
    }
  }

  _lastState = _currentState;
}

bool MIDIButton::isPressed() {
  return _currentState == LOW;
}

bool MIDIButton::isRisingEdge() {
  return _currentState == LOW && _lastState == HIGH;
}

bool MIDIButton::isFallingEdge() {
  return _currentState == HIGH && _lastState == LOW;
}