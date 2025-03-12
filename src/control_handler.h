/**
 * @file control_handler.h
 * @brief header for handling control mechanisms like encoders and buttons
 *
 * contains declarations for managing main encoder and MIDI buttons,
 * providing functions to update and set encoder values
 *
 * @author oddhorse (John Trinh)
 * @copyright copyright (c) 2025 oddhorse
 */

// TODO: doc this file, verify header comment is correct

#ifndef CONTROL_HANDLER_H
#define CONTROL_HANDLER_H

#include <vector>
#include "MIDIButton.h"

namespace Ctrls
{
    /**
     * @brief current encoder position.
     */
    extern volatile int encoderPosition;

    /**
     * @brief timestamp when encoder last changed.
     */
    extern unsigned long timeOfLastChange;

    /**
     * @brief all MIDI buttons registered in system.
     */
    extern std::vector<MIDIButton> buttons;

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
}
#endif // CONTROL_HANDLER_H