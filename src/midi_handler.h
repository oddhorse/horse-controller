#ifndef MIDI_HANDLER_H
#define MIDI_HANDLER_H

#include <Arduino.h>
#include <MIDI.h>
#include <bluefruit.h>

extern BLEMidi blemidi;
extern midi::MidiInterface<midi::SerialMIDI<BLEMidi>> MIDI;

#define midiReady() (Bluefruit.connected() && blemidi.notifyEnabled())

void setupMidi();
void handleNoteOn(byte channel, byte pitch, byte velocity);
void handleNoteOff(byte channel, byte pitch, byte velocity);
void midiRead();


#endif