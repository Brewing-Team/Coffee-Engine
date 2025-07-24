#pragma once

#include <memory>
#include <cassert>

#ifdef COFFEE_DEBUG
    #define COFFEE_ASSERTS
#endif

#define COFFEE_EXPAND_MACRO(x) x
#define COFFEE_STRINGIFY_MACRO(x) #x

#define BIT(x) (1 << x)

#define COFFEE_BIND_EVENT_FN(fn) [this](auto&&... args) -> decltype(auto) { return this->fn(std::forward<decltype(args)>(args)...); }

namespace Coffee {

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

//#include "CoffeeEngine/Core/Log.h"
//#include "CoffeeEngine/Core/Assert.h"
