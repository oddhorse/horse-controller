#ifndef DISPLAY_HANDLER_H
#define DISPLAY_HANDLER_H

#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <vector>

#include "MIDIButton.h"

extern std::vector<MIDIButton> buttons;

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 32 // OLED display height, in pixels

#define OLED_RESET     -1 // Reset pin # (or -1 if sharing Arduino reset pin)
#define SCREEN_ADDRESS 0x3C ///< See datasheet for Address; 0x3D for 128x64, 0x3C for 128x32

#define WHITE SSD1306_WHITE
#define BLACK SSD1306_BLACK

void setupDisplay();
void awaitingConnectionLoop(void *pvParameters);
void startDisplayTask(TaskFunction_t task);
void stopDisplayTask();
void updateButtonDisplay(void *pvParameters);



#endif