/**
 * @defgroup core Core
 * @brief Core components of the CoffeeEngine.
 * @{
 */

#pragma once

#include <cstdint>

namespace Coffee
{
    using MouseCode = uint16_t;

    namespace Mouse
    {
        /**
         * @enum Mouse
         * @brief Enumeration of mouse button codes.
         *
         * These values are from SDL\_mouse.h.
         */
        enum : MouseCode
        {
            BUTTON_LEFT =    1, ///< Left mouse button
            BUTTON_MIDDLE =  2, ///< Middle mouse button
            BUTTON_RIGHT =   3, ///< Right mouse button
            BUTTON_X1  =     4, ///< Extra mouse button 1
            BUTTON_X2  =     5  ///< Extra mouse button 2
        };
    }
}

/** @} */