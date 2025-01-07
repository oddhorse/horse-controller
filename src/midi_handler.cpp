#include "midi_handler.h"
//#include <bluefruit.h>


BLEMidi blemidi;
MIDI_CREATE_BLE_INSTANCE(blemidi);

void setupMidi() {
    // listen to all channels; also calls blemidi's begin()
    MIDI.begin(MIDI_CHANNEL_OMNI);
    // attach handlers to midi note receiving
    MIDI.setHandleNoteOn(handleNoteOn);
    MIDI.setHandleNoteOff(handleNoteOff);
    // Start MIDI read loop
    Scheduler.startLoop(midiRead);
}

void handleNoteOn(byte channel, byte pitch, byte velocity) {
    Serial.printf("Note on: channel = %d, pitch = %d, velocity = %d", channel, pitch, velocity);
    Serial.println();
}

void handleNoteOff(byte channel, byte pitch, byte velocity) {
    Serial.printf("Note off: channel = %d, pitch = %d, velocity = %d", channel, pitch, velocity);
    Serial.println();
}

void midiRead() {
    if (midiReady()) MIDI.read();
}