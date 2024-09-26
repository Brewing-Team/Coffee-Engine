#pragma once

#include "CoffeeEngine/Core/Base.h"
#include "CoffeeEngine/Core/Log.h"
#include <filesystem>

#ifdef COFFEE_ASSERTS

	// Alteratively we could use the same "default" message for both "WITH_MSG" and "NO_MSG" and
	// provide support for custom formatting by concatenating the formatting string instead of having the format inside the default message
	#define COFFEE_INTERNAL_ASSERT_IMPL(type, check, msg, ...) { if(!(check)) { COFFEE##type##ERROR(msg, __VA_ARGS__); } }
	#define COFFEE_INTERNAL_ASSERT_WITH_MSG(type, check, ...) COFFEE_INTERNAL_ASSERT_IMPL(type, check, "Assertion failed: {0}", __VA_ARGS__)
	#define COFFEE_INTERNAL_ASSERT_NO_MSG(type, check) COFFEE_INTERNAL_ASSERT_IMPL(type, check, "Assertion '{0}' failed at {1}:{2}", COFFEE_STRINGIFY_MACRO(check), std::filesystem::path(__FILE__).filename().string(), __LINE__)

	#define COFFEE_INTERNAL_ASSERT_GET_MACRO_NAME(arg1, arg2, macro, ...) macro
	#define COFFEE_INTERNAL_ASSERT_GET_MACRO(...) COFFEE_EXPAND_MACRO( COFFEE_INTERNAL_ASSERT_GET_MACRO_NAME(__VA_ARGS__, COFFEE_INTERNAL_ASSERT_WITH_MSG, COFFEE_INTERNAL_ASSERT_NO_MSG) )

	// Currently accepts at least the condition and one additional parameter (the message) being optional
	#define COFFEE_ASSERT(...) COFFEE_EXPAND_MACRO( COFFEE_INTERNAL_ASSERT_GET_MACRO(__VA_ARGS__)(_, __VA_ARGS__) )
	#define COFFEE_CORE_ASSERT(...) COFFEE_EXPAND_MACRO( COFFEE_INTERNAL_ASSERT_GET_MACRO(__VA_ARGS__)(_CORE_, __VA_ARGS__) )
#else
	#define COFFEE_ASSERT(...)
	#define COFFEE_CORE_ASSERT(...)
#endif
