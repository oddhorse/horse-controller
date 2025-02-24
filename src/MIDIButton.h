#ifndef MIDIBUTTON_H
#define MIDIBUTTON_H

#include <Arduino.h>
#include <MIDI.h>
#include <bluefruit.h>
#include <Bounce2.h>

#define MIDI_BUTTON_TYPE_NOTE 0
#define MIDI_BUTTON_TYPE_CC 1

extern midi::MidiInterface<midi::SerialMIDI<BLEMidi>> MIDI;

class MIDIButton {
public:
  MIDIButton(uint8_t pin, byte dataValue, byte channel, byte buttonType);

  void update();
  bool isPressed();
  bool isRisingEdge();
  bool isFallingEdge();

private:
  uint8_t _pin;
  byte _dataValue;
  byte _channel;
  byte _buttonType;
  bool _currentState;
  bool _lastState;
  Bounce _debouncer;
};

#endif