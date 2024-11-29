#include "LuaBackend.h"

#include "CoffeeEngine/Core/Input.h"
#include "CoffeeEngine/Core/KeyCodes.h"
#include "CoffeeEngine/Core/Log.h"
#include "CoffeeEngine/Core/MouseCodes.h"
#include "CoffeeEngine/Scene/Components.h"
#include "CoffeeEngine/Scene/Entity.h"

namespace Coffee {

    void BindKeyCodesToLua(sol::state& lua, sol::table& inputTable)
    {
        std::vector<std::pair<std::string, Coffee::KeyCode>> keyCodes = {
            {"UNKNOWN", Coffee::Key::UNKNOWN},
            {"A", Coffee::Key::A},
            {"B", Coffee::Key::B},
            {"C", Coffee::Key::C},
            {"D", Coffee::Key::D},
            {"E", Coffee::Key::E},
            {"F", Coffee::Key::F},
            {"G", Coffee::Key::G},
            {"H", Coffee::Key::H},
            {"I", Coffee::Key::I},
            {"J", Coffee::Key::J},
            {"K", Coffee::Key::K},
            {"L", Coffee::Key::L},
            {"M", Coffee::Key::M},
            {"N", Coffee::Key::N},
            {"O", Coffee::Key::O},
            {"P", Coffee::Key::P},
            {"Q", Coffee::Key::Q},
            {"R", Coffee::Key::R},
            {"S", Coffee::Key::S},
            {"T", Coffee::Key::T},
            {"U", Coffee::Key::U},
            {"V", Coffee::Key::V},
            {"W", Coffee::Key::W},
            {"X", Coffee::Key::X},
            {"Y", Coffee::Key::Y},
            {"Z", Coffee::Key::Z},
            {"D1", Coffee::Key::D1},
            {"D2", Coffee::Key::D2},
            {"D3", Coffee::Key::D3},
            {"D4", Coffee::Key::D4},
            {"D5", Coffee::Key::D5},
            {"D6", Coffee::Key::D6},
            {"D7", Coffee::Key::D7},
            {"D8", Coffee::Key::D8},
            {"D9", Coffee::Key::D9},
            {"D0", Coffee::Key::D0},
            {"RETURN", Coffee::Key::RETURN},
            {"ESCAPE", Coffee::Key::ESCAPE},
            {"BACKSPACE", Coffee::Key::BACKSPACE},
            {"TAB", Coffee::Key::TAB},
            {"SPACE", Coffee::Key::SPACE},
            {"MINUS", Coffee::Key::MINUS},
            {"EQUALS", Coffee::Key::EQUALS},
            {"LEFTBRACKET", Coffee::Key::LEFTBRACKET},
            {"RIGHTBRACKET", Coffee::Key::RIGHTBRACKET},
            {"BACKSLASH", Coffee::Key::BACKSLASH},
            {"NONUSHASH", Coffee::Key::NONUSHASH},
            {"SEMICOLON", Coffee::Key::SEMICOLON},
            {"APOSTROPHE", Coffee::Key::APOSTROPHE},
            {"GRAVE", Coffee::Key::GRAVE},
            {"COMMA", Coffee::Key::COMMA},
            {"PERIOD", Coffee::Key::PERIOD},
            {"SLASH", Coffee::Key::SLASH},
            {"CAPSLOCK", Coffee::Key::CAPSLOCK},
            {"F1", Coffee::Key::F1},
            {"F2", Coffee::Key::F2},
            {"F3", Coffee::Key::F3},
            {"F4", Coffee::Key::F4},
            {"F5", Coffee::Key::F5},
            {"F6", Coffee::Key::F6},
            {"F7", Coffee::Key::F7},
            {"F8", Coffee::Key::F8},
            {"F9", Coffee::Key::F9},
            {"F10", Coffee::Key::F10},
            {"F11", Coffee::Key::F11},
            {"F12", Coffee::Key::F12},
            {"PRINTSCREEN", Coffee::Key::PRINTSCREEN},
            {"SCROLLLOCK", Coffee::Key::SCROLLLOCK},
            {"PAUSE", Coffee::Key::PAUSE},
            {"INSERT", Coffee::Key::INSERT},
            {"HOME", Coffee::Key::HOME},
            {"PAGEUP", Coffee::Key::PAGEUP},
            {"DELETE", Coffee::Key::DELETE},
            {"END", Coffee::Key::END},
            {"PAGEDOWN", Coffee::Key::PAGEDOWN},
            {"RIGHT", Coffee::Key::RIGHT},
            {"LEFT", Coffee::Key::LEFT},
            {"DOWN", Coffee::Key::DOWN},
            {"UP", Coffee::Key::UP},
            {"NUMLOCKCLEAR", Coffee::Key::NUMLOCKCLEAR},
            {"KP_DIVIDE", Coffee::Key::KP_DIVIDE},
            {"KP_MULTIPLY", Coffee::Key::KP_MULTIPLY},
            {"KP_MINUS", Coffee::Key::KP_MINUS},
            {"KP_PLUS", Coffee::Key::KP_PLUS},
            {"KP_ENTER", Coffee::Key::KP_ENTER},
            {"KP_1", Coffee::Key::KP_1},
            {"KP_2", Coffee::Key::KP_2},
            {"KP_3", Coffee::Key::KP_3},
            {"KP_4", Coffee::Key::KP_4},
            {"KP_5", Coffee::Key::KP_5},
            {"KP_6", Coffee::Key::KP_6},
            {"KP_7", Coffee::Key::KP_7},
            {"KP_8", Coffee::Key::KP_8},
            {"KP_9", Coffee::Key::KP_9},
            {"KP_0", Coffee::Key::KP_0},
            {"KP_PERIOD", Coffee::Key::KP_PERIOD},
            {"NONUSBACKSLASH", Coffee::Key::NONUSBACKSLASH},
            {"APPLICATION", Coffee::Key::APPLICATION},
            {"POWER", Coffee::Key::POWER},
            {"KP_EQUALS", Coffee::Key::KP_EQUALS},
            {"F13", Coffee::Key::F13},
            {"F14", Coffee::Key::F14},
            {"F15", Coffee::Key::F15},
            {"F16", Coffee::Key::F16},
            {"F17", Coffee::Key::F17},
            {"F18", Coffee::Key::F18},
            {"F19", Coffee::Key::F19},
            {"F20", Coffee::Key::F20},
            {"F21", Coffee::Key::F21},
            {"F22", Coffee::Key::F22},
            {"F23", Coffee::Key::F23},
            {"F24", Coffee::Key::F24},
            {"EXECUTE", Coffee::Key::EXECUTE},
            {"HELP", Coffee::Key::HELP},
            {"MENU", Coffee::Key::MENU},
            {"SELECT", Coffee::Key::SELECT},
            {"STOP", Coffee::Key::STOP},
            {"AGAIN", Coffee::Key::AGAIN},
            {"UNDO", Coffee::Key::UNDO},
            {"CUT", Coffee::Key::CUT},
            {"COPY", Coffee::Key::COPY},
            {"PASTE", Coffee::Key::PASTE},
            {"FIND", Coffee::Key::FIND},
            {"MUTE", Coffee::Key::MUTE},
            {"VOLUMEUP", Coffee::Key::VOLUMEUP},
            {"VOLUMEDOWN", Coffee::Key::VOLUMEDOWN},
            {"KP_COMMA", Coffee::Key::KP_COMMA},
            {"KP_EQUALSAS400", Coffee::Key::KP_EQUALSAS400},
            {"INTERNATIONAL1", Coffee::Key::INTERNATIONAL1},
            {"INTERNATIONAL2", Coffee::Key::INTERNATIONAL2},
            {"INTERNATIONAL3", Coffee::Key::INTERNATIONAL3},
            {"INTERNATIONAL4", Coffee::Key::INTERNATIONAL4},
            {"INTERNATIONAL5", Coffee::Key::INTERNATIONAL5},
            {"INTERNATIONAL6", Coffee::Key::INTERNATIONAL6},
            {"INTERNATIONAL7", Coffee::Key::INTERNATIONAL7},
            {"INTERNATIONAL8", Coffee::Key::INTERNATIONAL8},
            {"INTERNATIONAL9", Coffee::Key::INTERNATIONAL9},
            {"LANG1", Coffee::Key::LANG1},
            {"LANG2", Coffee::Key::LANG2},
            {"LANG3", Coffee::Key::LANG3},
            {"LANG4", Coffee::Key::LANG4},
            {"LANG5", Coffee::Key::LANG5},
            {"LANG6", Coffee::Key::LANG6},
            {"LANG7", Coffee::Key::LANG7},
            {"LANG8", Coffee::Key::LANG8},
            {"LANG9", Coffee::Key::LANG9},
            {"ALTERASE", Coffee::Key::ALTERASE},
            {"SYSREQ", Coffee::Key::SYSREQ},
            {"CANCEL", Coffee::Key::CANCEL},
            {"CLEAR", Coffee::Key::CLEAR},
            {"PRIOR", Coffee::Key::PRIOR},
            {"RETURN2", Coffee::Key::RETURN2},
            {"SEPARATOR", Coffee::Key::SEPARATOR},
            {"OUT", Coffee::Key::OUT},
            {"OPER", Coffee::Key::OPER},
            {"CLEARAGAIN", Coffee::Key::CLEARAGAIN},
            {"CRSEL", Coffee::Key::CRSEL},
            {"EXSEL", Coffee::Key::EXSEL},
            {"KP_00", Coffee::Key::KP_00},
            {"KP_000", Coffee::Key::KP_000},
            {"THOUSANDSSEPARATOR", Coffee::Key::THOUSANDSSEPARATOR},
            {"DECIMALSEPARATOR", Coffee::Key::DECIMALSEPARATOR},
            {"CURRENCYUNIT", Coffee::Key::CURRENCYUNIT},
            {"CURRENCYSUBUNIT", Coffee::Key::CURRENCYSUBUNIT},
            {"KP_LEFTPAREN", Coffee::Key::KP_LEFTPAREN},
            {"KP_RIGHTPAREN", Coffee::Key::KP_RIGHTPAREN},
            {"KP_LEFTBRACE", Coffee::Key::KP_LEFTBRACE},
            {"KP_RIGHTBRACE", Coffee::Key::KP_RIGHTBRACE},
            {"KP_TAB", Coffee::Key::KP_TAB},
            {"KP_BACKSPACE", Coffee::Key::KP_BACKSPACE},
            {"KP_A", Coffee::Key::KP_A},
            {"KP_B", Coffee::Key::KP_B},
            {"KP_C", Coffee::Key::KP_C},
            {"KP_D", Coffee::Key::KP_D},
            {"KP_E", Coffee::Key::KP_E},
            {"KP_F", Coffee::Key::KP_F},
            {"KP_XOR", Coffee::Key::KP_XOR},
            {"KP_POWER", Coffee::Key::KP_POWER},
            {"KP_PERCENT", Coffee::Key::KP_PERCENT},
            {"KP_LESS", Coffee::Key::KP_LESS},
            {"KP_GREATER", Coffee::Key::KP_GREATER},
            {"KP_AMPERSAND", Coffee::Key::KP_AMPERSAND},
            {"KP_DBLAMPERSAND", Coffee::Key::KP_DBLAMPERSAND},
            {"KP_VERTICALBAR", Coffee::Key::KP_VERTICALBAR},
            {"KP_DBLVERTICALBAR", Coffee::Key::KP_DBLVERTICALBAR},
            {"KP_COLON", Coffee::Key::KP_COLON},
            {"KP_HASH", Coffee::Key::KP_HASH},
            {"KP_SPACE", Coffee::Key::KP_SPACE},
            {"KP_AT", Coffee::Key::KP_AT},
            {"KP_EXCLAM", Coffee::Key::KP_EXCLAM},
            {"KP_MEMSTORE", Coffee::Key::KP_MEMSTORE},
            {"KP_MEMRECALL", Coffee::Key::KP_MEMRECALL},
            {"KP_MEMCLEAR", Coffee::Key::KP_MEMCLEAR},
            {"KP_MEMADD", Coffee::Key::KP_MEMADD},
            {"KP_MEMSUBTRACT", Coffee::Key::KP_MEMSUBTRACT},
            {"KP_MEMMULTIPLY", Coffee::Key::KP_MEMMULTIPLY},
            {"KP_MEMDIVIDE", Coffee::Key::KP_MEMDIVIDE},
            {"KP_PLUSMINUS", Coffee::Key::KP_PLUSMINUS},
            {"KP_CLEAR", Coffee::Key::KP_CLEAR},
            {"KP_CLEARENTRY", Coffee::Key::KP_CLEARENTRY},
            {"KP_BINARY", Coffee::Key::KP_BINARY},
            {"KP_OCTAL", Coffee::Key::KP_OCTAL},
            {"KP_DECIMAL", Coffee::Key::KP_DECIMAL},
            {"KP_HEXADECIMAL", Coffee::Key::KP_HEXADECIMAL},
            {"LCTRL", Coffee::Key::LCTRL},
            {"LSHIFT", Coffee::Key::LSHIFT},
            {"LALT", Coffee::Key::LALT},
            {"LGUI", Coffee::Key::LGUI},
            {"RCTRL", Coffee::Key::RCTRL},
            {"RSHIFT", Coffee::Key::RSHIFT},
            {"RALT", Coffee::Key::RALT},
            {"RGUI", Coffee::Key::RGUI},
            {"MODE", Coffee::Key::MODE},
            {"SLEEP", Coffee::Key::SLEEP},
            {"WAKE", Coffee::Key::WAKE},
            {"CHANNEL_INCREMENT", Coffee::Key::CHANNEL_INCREMENT},
            {"CHANNEL_DECREMENT", Coffee::Key::CHANNEL_DECREMENT},
            {"MEDIA_PLAY", Coffee::Key::MEDIA_PLAY},
            {"MEDIA_PAUSE", Coffee::Key::MEDIA_PAUSE},
            {"MEDIA_RECORD", Coffee::Key::MEDIA_RECORD},
            {"MEDIA_FAST_FORWARD", Coffee::Key::MEDIA_FAST_FORWARD},
            {"MEDIA_REWIND", Coffee::Key::MEDIA_REWIND},
            {"MEDIA_NEXT_TRACK", Coffee::Key::MEDIA_NEXT_TRACK},
            {"MEDIA_PREVIOUS_TRACK", Coffee::Key::MEDIA_PREVIOUS_TRACK},
            {"MEDIA_STOP", Coffee::Key::MEDIA_STOP},
            {"MEDIA_EJECT", Coffee::Key::MEDIA_EJECT},
            {"MEDIA_PLAY_PAUSE", Coffee::Key::MEDIA_PLAY_PAUSE},
            {"MEDIA_SELECT", Coffee::Key::MEDIA_SELECT},
            {"AC_NEW", Coffee::Key::AC_NEW},
            {"AC_OPEN", Coffee::Key::AC_OPEN},
            {"AC_CLOSE", Coffee::Key::AC_CLOSE},
            {"AC_EXIT", Coffee::Key::AC_EXIT},
            {"AC_SAVE", Coffee::Key::AC_SAVE},
            {"AC_PRINT", Coffee::Key::AC_PRINT},
            {"AC_PROPERTIES", Coffee::Key::AC_PROPERTIES},
            {"AC_SEARCH", Coffee::Key::AC_SEARCH},
            {"AC_HOME", Coffee::Key::AC_HOME},
            {"AC_BACK", Coffee::Key::AC_BACK},
            {"AC_FORWARD", Coffee::Key::AC_FORWARD},
            {"AC_STOP", Coffee::Key::AC_STOP},
            {"AC_REFRESH", Coffee::Key::AC_REFRESH},
            {"AC_BOOKMARKS", Coffee::Key::AC_BOOKMARKS},
            {"SOFTLEFT", Coffee::Key::SOFTLEFT},
            {"SOFTRIGHT", Coffee::Key::SOFTRIGHT},
            {"CALL", Coffee::Key::CALL},
            {"ENDCALL", Coffee::Key::ENDCALL}
        };

        sol::table keyCodeTable = lua.create_table();
        for (const auto& keyCode : keyCodes) {
            keyCodeTable[keyCode.first] = keyCode.second;
        }
        inputTable["keycode"] = keyCodeTable;
    }

    void BindMouseCodesToLua(sol::state& lua, sol::table& inputTable)
    {
        std::vector<std::pair<std::string, Coffee::MouseCode>> mouseCodes = {
            {"LEFT", Coffee::Mouse::BUTTON_LEFT},
            {"MIDDLE", Coffee::Mouse::BUTTON_MIDDLE},
            {"RIGHT", Coffee::Mouse::BUTTON_RIGHT},
            {"X1", Coffee::Mouse::BUTTON_X1},
            {"X2", Coffee::Mouse::BUTTON_X2}
        };

        sol::table mouseCodeTable = lua.create_table();
        for (const auto& mouseCode : mouseCodes) {
            mouseCodeTable[mouseCode.first] = mouseCode.second;
        }
        inputTable["mousecode"] = mouseCodeTable;
    }

    void LuaBackend::Initialize() {
        luaState.open_libraries(sol::lib::base, sol::lib::math, sol::lib::string, sol::lib::table);

        # pragma region Bind Log Functions
        luaState.set_function("log", [](const std::string& message) {
            COFFEE_CORE_INFO("{0}", message);
        });

        luaState.set_function("log_warning", [](const std::string& message) {
            COFFEE_CORE_WARN("{0}", message);
        });

        luaState.set_function("log_error", [](const std::string& message) {
            COFFEE_CORE_ERROR("{0}", message);
        });

        luaState.set_function("log_critical", [](const std::string& message) {
            COFFEE_CORE_CRITICAL("{0}", message);
        });

        # pragma endregion

        # pragma region Bind Input Functions
        sol::table inputTable = luaState.create_table();
        BindKeyCodesToLua(luaState, inputTable);
        BindMouseCodesToLua(luaState, inputTable);

        inputTable.set_function("is_key_pressed", [](KeyCode key) {
            return Coffee::Input::IsKeyPressed(key);
        });

        inputTable.set_function("is_mouse_button_pressed", [](MouseCode button) {
            return Coffee::Input::IsMouseButtonPressed(button);
        });

        inputTable.set_function("get_mouse_position", []() {
            glm::vec2 mousePosition = Coffee::Input::GetMousePosition();
            return std::make_tuple(mousePosition.x, mousePosition.y);
        });

        luaState["input"] = inputTable;
        # pragma endregion

        # pragma region Bind Timer Functions
        # pragma endregion

        #pragma region Bind Entity Functions
        #pragma endregion

        # pragma region Bind Components Functions
        luaState.new_usertype<Coffee::TagComponent>("tag_component",
            sol::constructors<Coffee::TagComponent(), Coffee::TagComponent(const std::string&)>(),
            "tag", &Coffee::TagComponent::Tag
        );

        luaState.new_usertype<Coffee::TransformComponent>("transform_component",
            sol::constructors<Coffee::TransformComponent(), Coffee::TransformComponent(const glm::vec3&)>(),
            "position", &Coffee::TransformComponent::Position,
            "rotation", &Coffee::TransformComponent::Rotation,
            "scale", &Coffee::TransformComponent::Scale,
            "get_local_transform", &Coffee::TransformComponent::GetLocalTransform,
            "set_local_transform", &Coffee::TransformComponent::SetLocalTransform,
            "get_world_transform", &Coffee::TransformComponent::GetWorldTransform,
            "set_world_transform", &Coffee::TransformComponent::SetWorldTransform
        );

        luaState.new_usertype<Coffee::CameraComponent>("camera_component",
            sol::constructors<Coffee::CameraComponent()>(),
            "camera", &Coffee::CameraComponent::Camera
        );

        luaState.new_usertype<Coffee::MeshComponent>("mesh_component",
            sol::constructors<Coffee::MeshComponent(), Coffee::MeshComponent(Coffee::Ref<Coffee::Mesh>)>(),
            "mesh", &Coffee::MeshComponent::mesh,
            "drawAABB", &Coffee::MeshComponent::drawAABB,
            "get_mesh", &Coffee::MeshComponent::GetMesh
        );

        luaState.new_usertype<Coffee::MaterialComponent>("material_component",
            sol::constructors<Coffee::MaterialComponent(), Coffee::MaterialComponent(Coffee::Ref<Coffee::Material>)>(),
            "material", &Coffee::MaterialComponent::material
        );

        luaState.new_usertype<Coffee::LightComponent>("light_component",
            sol::constructors<Coffee::LightComponent()>(),
            "color", &Coffee::LightComponent::Color,
            "direction", &Coffee::LightComponent::Direction,
            "position", &Coffee::LightComponent::Position,
            "range", &Coffee::LightComponent::Range,
            "attenuation", &Coffee::LightComponent::Attenuation,
            "intensity", &Coffee::LightComponent::Intensity,
            "angle", &Coffee::LightComponent::Angle,
            "type", &Coffee::LightComponent::type
        );
        # pragma endregion

    }

    void LuaBackend::ExecuteScript(const std::string& script) {
        try {
            luaState.script(script);
        } catch (const sol::error& e) {
            COFFEE_CORE_ERROR("[Lua Error]: {0}", e.what());
        }
    }

    void LuaBackend::ExecuteFile(const std::filesystem::path& filepath) {
        try {
            luaState.script_file(filepath.string());
        } catch (const sol::error& e) {
            COFFEE_CORE_ERROR("[Lua Error]: {0}", e.what());
        }
    }

    void LuaBackend::RegisterFunction(std::function<void()> func, const std::string& name)
    {
        luaState.set_function(name, func);
        COFFEE_CORE_INFO("Registered Lua function {0}", name);
    }

    void LuaBackend::BindFunction(const std::string& name, std::function<void()>& func)
    {
        func = luaState[name];
        COFFEE_CORE_INFO("Bound Lua function {0}", name);
    }
} // namespace Coffee