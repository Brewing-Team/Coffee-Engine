#ifndef COFFEE_PCH_H
#define COFFEE_PCH_H

// System headers first - most expensive ones early
#include <cmath>     // 25932ms total, include very early
#include <vector>    // 12068ms total  
#include <string>    // 11166ms total
#include <iostream>
#include <memory>
#include <utility>
#include <algorithm>
#include <functional>

// STL containers
#include <sstream>
#include <array>
#include <unordered_map>
#include <unordered_set>

// Third-party heavy headers
#include <entt/entt.hpp>  // Very expensive, include in PCH

// GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/quaternion.hpp>
#include <glm/gtx/matrix_decompose.hpp>

// Cereal - but consider reducing template instantiations
#include <cereal/cereal.hpp>
#include <cereal/types/memory.hpp>
#include <cereal/types/vector.hpp>
#include <cereal/types/string.hpp>
#include <cereal/types/unordered_map.hpp>
#include <cereal/types/unordered_set.hpp>
#include <cereal/types/optional.hpp>
#include <cereal/types/variant.hpp>
#include <cereal/types/array.hpp>
#include <cereal/types/tuple.hpp>
#include <cereal/types/complex.hpp>
#include <cereal/types/polymorphic.hpp>
#include <cereal/archives/json.hpp>
#include <cereal/archives/binary.hpp>
#include <cereal/access.hpp>

// Core engine headers
#include "CoffeeEngine/Core/Base.h"
#include "CoffeeEngine/Core/Log.h"
#include "CoffeeEngine/Core/KeyCodes.h"
#include "CoffeeEngine/Core/MouseCodes.h"
#include "CoffeeEngine/Core/ControllerCodes.h"

// Events
#include "CoffeeEngine/Events/Event.h"
#include "CoffeeEngine/Events/KeyEvent.h"
#include "CoffeeEngine/Events/MouseEvent.h"
#include "CoffeeEngine/Events/ControllerEvent.h"

// Input
#include "CoffeeEngine/Input/InputBinding.h"
#include "CoffeeEngine/Input/Gamepad.h"

// Serialization
#include "CoffeeEngine/IO/Serialization/FilesystemPathSerialization.h"
#include "CoffeeEngine/IO/Serialization/GLMSerialization.h"

// IO
#include "CoffeeEngine/IO/ResourceImporter.h"
#include "CoffeeEngine/IO/ResourceRegistry.h"
#include "CoffeeEngine/IO/ResourceLoader.h"
#include "CoffeeEngine/IO/ResourceSaver.h"
#include "CoffeeEngine/IO/Resource.h"
#include "CoffeeEngine/IO/ResourceFormat.h"
#include "CoffeeEngine/IO/ResourceUtils.h"
#include "CoffeeEngine/IO/ImportData/ImportData.h"
#include "CoffeeEngine/IO/ImportData/ImportDataUtils.h"
#include "CoffeeEngine/IO/CacheManager.h"

// Core systems
#include "CoffeeEngine/Core/SystemInfo.h"

// Scene system (very expensive but frequently used)
#include "CoffeeEngine/Scene/Entity.h"
#include "CoffeeEngine/Scene/Scene.h"
#include "CoffeeEngine/Scene/Components.h"  // 8333ms total, very expensive

// SDL3
#include <SDL3/SDL_events.h>
#include <SDL3/SDL_keyboard.h>
#include <SDL3/SDL_mouse.h>

// Ozz Animation - expensive math headers
#include <ozz/animation/runtime/local_to_model_job.h>
#include <ozz/animation/runtime/sampling_job.h>
#include <ozz/animation/runtime/blending_job.h>
#include <ozz/animation/runtime/skeleton.h>
#include <ozz/animation/runtime/animation.h>
#include <ozz/base/memory/unique_ptr.h>
#include <ozz/base/maths/soa_transform.h>

// Sol2 Lua bindings (very expensive templates - 51+ seconds)
#include <sol/sol.hpp>

// ImGui headers (expensive and frequently included)
#include "imgui.h"
#include "imgui_internal.h"  // 9097ms total, 1137ms avg

// Expensive but frequently used engine headers
#include "CoffeeEngine/Scripting/Lua/LuaBackend.h"  // 8365ms total, 1673ms avg

// MSDF Atlas Gen (expensive)
#include "msdfgen/msdfgen.h"  // 7575ms total, 505ms avg

#endif // COFFEE_PCH_H