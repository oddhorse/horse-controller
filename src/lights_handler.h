#ifndef LIGHTS_HANDLER_H
#define LIGHTS_HANDLER_H

#include <Arduino.h>
#include <Adafruit_DotStar.h>

// define constants
#define DOTSTAR_DATA_PIN 8
#define DOTSTAR_CLOCK_PIN 6
// this one is defined
//#define LED_BUILTIN 3

// define the heartbeat variables
#define HEARTBEAT_STRENGTH_DISCONNECTED 1.0
#define HEARTBEAT_STRENGTH_CONNECTED 0.2
#define HEARTBEAT_SPEED_DISCONNECTED 6
#define HEARTBEAT_SPEED_CONNECTED 0.75

void setupLights();
void updateDotstar();
void updateLED();
void updateLastButtonPress();
void flashConnected();
void flashDisconnected();


#endif