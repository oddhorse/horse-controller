#ifndef DISPLAY_HANDLER_H
#define DISPLAY_HANDLER_H

// TODO: doc this file, write header comment

#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <vector>

#include "MIDIButton.h"

namespace Display
{
    void setupDisplay();
    void awaitingConnectionLoop(void *pvParameters);
    void startDisplayTask(TaskFunction_t task);
    void stopDisplayTask();
    void updateButtonDisplay(void *pvParameters);
}
#endif