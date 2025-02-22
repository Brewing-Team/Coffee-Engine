#pragma once

#include <cstdint>
namespace Coffee
{

    using ButtonCode = uint16_t;
    using AxisCode = uint16_t;

    namespace Button
    {
        enum : ButtonCode
        {
            // From SDL_gamepad.h

            Invalid = 0,
            South,           /**< Bottom face button (e.g. Xbox A button) */
            East,            /**< Right face button (e.g. Xbox B button) */
            West,            /**< Left face button (e.g. Xbox X button) */
            North,           /**< Top face button (e.g. Xbox Y button) */
            Back,
            Guide,
            Start,
            LeftStick,
            RightStick,
            LeftShoulder,
            RightShoulder,
            DpadUp,
            DpadDown,
            DpadLeft,
            DpadRight,
            Misc1,           /**< Additional button (e.g. Xbox Series X share button, PS5 microphone button, Nintendo Switch Pro capture button, Amazon Luna microphone button, Google Stadia capture button) */
            RightPaddle1,   /**< Upper or primary paddle, under your right hand (e.g. Xbox Elite paddle P1) */
            LeftPaddle1,    /**< Upper or primary paddle, under your left hand (e.g. Xbox Elite paddle P3) */
            RightPaddle2,   /**< Lower or secondary paddle, under your right hand (e.g. Xbox Elite paddle P2) */
            Leftpaddle2,    /**< Lower or secondary paddle, under your left hand (e.g. Xbox Elite paddle P4) */
            Touchpad,        /**< PS4/PS5 touchpad button */
            Misc2,           /**< Additional button */
            Misc3,           /**< Additional button */
            Misc4,           /**< Additional button */
            Misc5,           /**< Additional button */
            Misc6,           /**< Additional button */
            Count
        };
    }

    namespace Axis
    {
        enum : AxisCode
        {
            // From SDL_gamepad.h

            Invalid = 0,
            LeftX,
            LeftY,
            RightX,
            RightY,
            LeftTrigger,
            RightTrigger,
            Count
        };
    }

};