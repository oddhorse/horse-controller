/**
 * @file control_handler.h
 * @brief Header file for handling control mechanisms such as encoders and buttons.
 *
 * This file contains declarations for managing the main encoder and MIDI buttons.
 * It provides functions to update and set the encoder values.
 */

#define CORE_INT12_PIN		12
#define CORE_INT13_PIN		13
#define ENCODER_OPTIMIZE_INTERRUPTS
#ifndef CONTROL_HANDLER_H
#define CONTROL_HANDLER_H

#include <vector>
#include "MIDIButton.h"

/**
 * @brief External declaration of the encoder position.
 */
extern volatile int encoderPosition;

/**
 * @brief External declaration of timestamp when encoder last changed.
 */
extern unsigned long timeOfLastChange;

/**
 * @brief External declaration of the vector of MIDI buttons.
 */
extern std::vector<MIDIButton> buttons;

void encoderISR();

void setupEncoder();

/**
 * @brief updates values of all registered buttons. meant to run in main loop.
 */
void updateControls();

/**
 * @brief Sets the encoder to a specific value.
 *
 * @param val The value to set the encoder to.
 */
void setEncoder(int val);

#endif // CONTROL_HANDLER_H