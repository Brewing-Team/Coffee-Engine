#pragma once

#include <memory>
#include <cassert>

#ifdef TEA_DEBUG
    #define TEA_ASSERTS
#endif

#define TEA_EXPAND_MACRO(x) x
#define TEA_STRINGIFY_MACRO(x) #x

#define BIT(x) (1 << x)

namespace Tea {

	template<typename T>
	using Scope = std::unique_ptr<T>;
	template<typename T, typename ... Args>
	constexpr Scope<T> CreateScope(Args&& ... args)
	{
		return std::make_unique<T>(std::forward<Args>(args)...);
	}

	template<typename T>
	using Ref = std::shared_ptr<T>;
	template<typename T, typename ... Args>
	constexpr Ref<T> CreateRef(Args&& ... args)
	{
		return std::make_shared<T>(std::forward<Args>(args)...);
	}

}

#include "TeaEngine/Core/Log.h"
#include "TeaEngine/Core/Assert.h"
