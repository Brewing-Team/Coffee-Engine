#pragma once

#include "TeaEngine/Core/Base.h"
#include "TeaEngine/Core/Log.h"
#include <filesystem>

#ifdef TEA_ASSERTS

	// Alteratively we could use the same "default" message for both "WITH_MSG" and "NO_MSG" and
	// provide support for custom formatting by concatenating the formatting string instead of having the format inside the default message
	#define TEA_INTERNAL_ASSERT_IMPL(type, check, msg, ...) { if(!(check)) { TEA##type##ERROR(msg, __VA_ARGS__); } }
	#define TEA_INTERNAL_ASSERT_WITH_MSG(type, check, ...) TEA_INTERNAL_ASSERT_IMPL(type, check, "Assertion failed: {0}", __VA_ARGS__)
	#define TEA_INTERNAL_ASSERT_NO_MSG(type, check) TEA_INTERNAL_ASSERT_IMPL(type, check, "Assertion '{0}' failed at {1}:{2}", TEA_STRINGIFY_MACRO(check), std::filesystem::path(__FILE__).filename().string(), __LINE__)

	#define TEA_INTERNAL_ASSERT_GET_MACRO_NAME(arg1, arg2, macro, ...) macro
	#define TEA_INTERNAL_ASSERT_GET_MACRO(...) TEA_EXPAND_MACRO( TEA_INTERNAL_ASSERT_GET_MACRO_NAME(__VA_ARGS__, TEA_INTERNAL_ASSERT_WITH_MSG, TEA_INTERNAL_ASSERT_NO_MSG) )

	// Currently accepts at least the condition and one additional parameter (the message) being optional
	#define TEA_ASSERT(...) TEA_EXPAND_MACRO( TEA_INTERNAL_ASSERT_GET_MACRO(__VA_ARGS__)(_, __VA_ARGS__) )
	#define TEA_CORE_ASSERT(...) TEA_EXPAND_MACRO( TEA_INTERNAL_ASSERT_GET_MACRO(__VA_ARGS__)(_CORE_, __VA_ARGS__) )
#else
	#define TEA_ASSERT(...)
	#define TEA_CORE_ASSERT(...)
#endif
