#pragma once

#include <cstdint>
namespace Coffee
{
	using MouseCode = uint16_t;

	namespace Mouse
	{
		enum : MouseCode
		{
			// From SDL_mouse.h
			BUTTON_LEFT =    1,
			BUTTON_MIDDLE =  2,
			BUTTON_RIGHT =   3,
			BUTTON_X1  =     4,
			BUTTON_X2  =     5
		};
	}
}
