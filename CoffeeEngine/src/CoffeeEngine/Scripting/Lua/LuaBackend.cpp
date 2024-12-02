#include "LuaBackend.h"

#include "CoffeeEngine/Core/Input.h"
#include "CoffeeEngine/Core/KeyCodes.h"
#include "CoffeeEngine/Core/Log.h"
#include "CoffeeEngine/Core/MouseCodes.h"
#include "CoffeeEngine/Scene/Components.h"
#include "CoffeeEngine/Scene/Entity.h"

#include <fstream>
#include <regex>

namespace Coffee {

    void BindKeyCodesToLua(sol::state& lua, sol::table& inputTable)
    {
        std::vector<std::pair<std::string, KeyCode>> keyCodes = {
            {"UNKNOWN", Key::UNKNOWN},
            {"A", Key::A},
            {"B", Key::B},
            {"C", Key::C},
            {"D", Key::D},
            {"E", Key::E},
            {"F", Key::F},
            {"G", Key::G},
            {"H", Key::H},
            {"I", Key::I},
            {"J", Key::J},
            {"K", Key::K},
            {"L", Key::L},
            {"M", Key::M},
            {"N", Key::N},
            {"O", Key::O},
            {"P", Key::P},
            {"Q", Key::Q},
            {"R", Key::R},
            {"S", Key::S},
            {"T", Key::T},
            {"U", Key::U},
            {"V", Key::V},
            {"W", Key::W},
            {"X", Key::X},
            {"Y", Key::Y},
            {"Z", Key::Z},
            {"D1", Key::D1},
            {"D2", Key::D2},
            {"D3", Key::D3},
            {"D4", Key::D4},
            {"D5", Key::D5},
            {"D6", Key::D6},
            {"D7", Key::D7},
            {"D8", Key::D8},
            {"D9", Key::D9},
            {"D0", Key::D0},
            {"RETURN", Key::RETURN},
            {"ESCAPE", Key::ESCAPE},
            {"BACKSPACE", Key::BACKSPACE},
            {"TAB", Key::TAB},
            {"SPACE", Key::SPACE},
            {"MINUS", Key::MINUS},
            {"EQUALS", Key::EQUALS},
            {"LEFTBRACKET", Key::LEFTBRACKET},
            {"RIGHTBRACKET", Key::RIGHTBRACKET},
            {"BACKSLASH", Key::BACKSLASH},
            {"NONUSHASH", Key::NONUSHASH},
            {"SEMICOLON", Key::SEMICOLON},
            {"APOSTROPHE", Key::APOSTROPHE},
            {"GRAVE", Key::GRAVE},
            {"COMMA", Key::COMMA},
            {"PERIOD", Key::PERIOD},
            {"SLASH", Key::SLASH},
            {"CAPSLOCK", Key::CAPSLOCK},
            {"F1", Key::F1},
            {"F2", Key::F2},
            {"F3", Key::F3},
            {"F4", Key::F4},
            {"F5", Key::F5},
            {"F6", Key::F6},
            {"F7", Key::F7},
            {"F8", Key::F8},
            {"F9", Key::F9},
            {"F10", Key::F10},
            {"F11", Key::F11},
            {"F12", Key::F12},
            {"PRINTSCREEN", Key::PRINTSCREEN},
            {"SCROLLLOCK", Key::SCROLLLOCK},
            {"PAUSE", Key::PAUSE},
            {"INSERT", Key::INSERT},
            {"HOME", Key::HOME},
            {"PAGEUP", Key::PAGEUP},
            {"DELETE", Key::DELETE},
            {"END", Key::END},
            {"PAGEDOWN", Key::PAGEDOWN},
            {"RIGHT", Key::RIGHT},
            {"LEFT", Key::LEFT},
            {"DOWN", Key::DOWN},
            {"UP", Key::UP},
            {"NUMLOCKCLEAR", Key::NUMLOCKCLEAR},
            {"KP_DIVIDE", Key::KP_DIVIDE},
            {"KP_MULTIPLY", Key::KP_MULTIPLY},
            {"KP_MINUS", Key::KP_MINUS},
            {"KP_PLUS", Key::KP_PLUS},
            {"KP_ENTER", Key::KP_ENTER},
            {"KP_1", Key::KP_1},
            {"KP_2", Key::KP_2},
            {"KP_3", Key::KP_3},
            {"KP_4", Key::KP_4},
            {"KP_5", Key::KP_5},
            {"KP_6", Key::KP_6},
            {"KP_7", Key::KP_7},
            {"KP_8", Key::KP_8},
            {"KP_9", Key::KP_9},
            {"KP_0", Key::KP_0},
            {"KP_PERIOD", Key::KP_PERIOD},
            {"NONUSBACKSLASH", Key::NONUSBACKSLASH},
            {"APPLICATION", Key::APPLICATION},
            {"POWER", Key::POWER},
            {"KP_EQUALS", Key::KP_EQUALS},
            {"F13", Key::F13},
            {"F14", Key::F14},
            {"F15", Key::F15},
            {"F16", Key::F16},
            {"F17", Key::F17},
            {"F18", Key::F18},
            {"F19", Key::F19},
            {"F20", Key::F20},
            {"F21", Key::F21},
            {"F22", Key::F22},
            {"F23", Key::F23},
            {"F24", Key::F24},
            {"EXECUTE", Key::EXECUTE},
            {"HELP", Key::HELP},
            {"MENU", Key::MENU},
            {"SELECT", Key::SELECT},
            {"STOP", Key::STOP},
            {"AGAIN", Key::AGAIN},
            {"UNDO", Key::UNDO},
            {"CUT", Key::CUT},
            {"COPY", Key::COPY},
            {"PASTE", Key::PASTE},
            {"FIND", Key::FIND},
            {"MUTE", Key::MUTE},
            {"VOLUMEUP", Key::VOLUMEUP},
            {"VOLUMEDOWN", Key::VOLUMEDOWN},
            {"KP_COMMA", Key::KP_COMMA},
            {"KP_EQUALSAS400", Key::KP_EQUALSAS400},
            {"INTERNATIONAL1", Key::INTERNATIONAL1},
            {"INTERNATIONAL2", Key::INTERNATIONAL2},
            {"INTERNATIONAL3", Key::INTERNATIONAL3},
            {"INTERNATIONAL4", Key::INTERNATIONAL4},
            {"INTERNATIONAL5", Key::INTERNATIONAL5},
            {"INTERNATIONAL6", Key::INTERNATIONAL6},
            {"INTERNATIONAL7", Key::INTERNATIONAL7},
            {"INTERNATIONAL8", Key::INTERNATIONAL8},
            {"INTERNATIONAL9", Key::INTERNATIONAL9},
            {"LANG1", Key::LANG1},
            {"LANG2", Key::LANG2},
            {"LANG3", Key::LANG3},
            {"LANG4", Key::LANG4},
            {"LANG5", Key::LANG5},
            {"LANG6", Key::LANG6},
            {"LANG7", Key::LANG7},
            {"LANG8", Key::LANG8},
            {"LANG9", Key::LANG9},
            {"ALTERASE", Key::ALTERASE},
            {"SYSREQ", Key::SYSREQ},
            {"CANCEL", Key::CANCEL},
            {"CLEAR", Key::CLEAR},
            {"PRIOR", Key::PRIOR},
            {"RETURN2", Key::RETURN2},
            {"SEPARATOR", Key::SEPARATOR},
            {"OUT", Key::OUT},
            {"OPER", Key::OPER},
            {"CLEARAGAIN", Key::CLEARAGAIN},
            {"CRSEL", Key::CRSEL},
            {"EXSEL", Key::EXSEL},
            {"KP_00", Key::KP_00},
            {"KP_000", Key::KP_000},
            {"THOUSANDSSEPARATOR", Key::THOUSANDSSEPARATOR},
            {"DECIMALSEPARATOR", Key::DECIMALSEPARATOR},
            {"CURRENCYUNIT", Key::CURRENCYUNIT},
            {"CURRENCYSUBUNIT", Key::CURRENCYSUBUNIT},
            {"KP_LEFTPAREN", Key::KP_LEFTPAREN},
            {"KP_RIGHTPAREN", Key::KP_RIGHTPAREN},
            {"KP_LEFTBRACE", Key::KP_LEFTBRACE},
            {"KP_RIGHTBRACE", Key::KP_RIGHTBRACE},
            {"KP_TAB", Key::KP_TAB},
            {"KP_BACKSPACE", Key::KP_BACKSPACE},
            {"KP_A", Key::KP_A},
            {"KP_B", Key::KP_B},
            {"KP_C", Key::KP_C},
            {"KP_D", Key::KP_D},
            {"KP_E", Key::KP_E},
            {"KP_F", Key::KP_F},
            {"KP_XOR", Key::KP_XOR},
            {"KP_POWER", Key::KP_POWER},
            {"KP_PERCENT", Key::KP_PERCENT},
            {"KP_LESS", Key::KP_LESS},
            {"KP_GREATER", Key::KP_GREATER},
            {"KP_AMPERSAND", Key::KP_AMPERSAND},
            {"KP_DBLAMPERSAND", Key::KP_DBLAMPERSAND},
            {"KP_VERTICALBAR", Key::KP_VERTICALBAR},
            {"KP_DBLVERTICALBAR", Key::KP_DBLVERTICALBAR},
            {"KP_COLON", Key::KP_COLON},
            {"KP_HASH", Key::KP_HASH},
            {"KP_SPACE", Key::KP_SPACE},
            {"KP_AT", Key::KP_AT},
            {"KP_EXCLAM", Key::KP_EXCLAM},
            {"KP_MEMSTORE", Key::KP_MEMSTORE},
            {"KP_MEMRECALL", Key::KP_MEMRECALL},
            {"KP_MEMCLEAR", Key::KP_MEMCLEAR},
            {"KP_MEMADD", Key::KP_MEMADD},
            {"KP_MEMSUBTRACT", Key::KP_MEMSUBTRACT},
            {"KP_MEMMULTIPLY", Key::KP_MEMMULTIPLY},
            {"KP_MEMDIVIDE", Key::KP_MEMDIVIDE},
            {"KP_PLUSMINUS", Key::KP_PLUSMINUS},
            {"KP_CLEAR", Key::KP_CLEAR},
            {"KP_CLEARENTRY", Key::KP_CLEARENTRY},
            {"KP_BINARY", Key::KP_BINARY},
            {"KP_OCTAL", Key::KP_OCTAL},
            {"KP_DECIMAL", Key::KP_DECIMAL},
            {"KP_HEXADECIMAL", Key::KP_HEXADECIMAL},
            {"LCTRL", Key::LCTRL},
            {"LSHIFT", Key::LSHIFT},
            {"LALT", Key::LALT},
            {"LGUI", Key::LGUI},
            {"RCTRL", Key::RCTRL},
            {"RSHIFT", Key::RSHIFT},
            {"RALT", Key::RALT},
            {"RGUI", Key::RGUI},
            {"MODE", Key::MODE},
            {"SLEEP", Key::SLEEP},
            {"WAKE", Key::WAKE},
            {"CHANNEL_INCREMENT", Key::CHANNEL_INCREMENT},
            {"CHANNEL_DECREMENT", Key::CHANNEL_DECREMENT},
            {"MEDIA_PLAY", Key::MEDIA_PLAY},
            {"MEDIA_PAUSE", Key::MEDIA_PAUSE},
            {"MEDIA_RECORD", Key::MEDIA_RECORD},
            {"MEDIA_FAST_FORWARD", Key::MEDIA_FAST_FORWARD},
            {"MEDIA_REWIND", Key::MEDIA_REWIND},
            {"MEDIA_NEXT_TRACK", Key::MEDIA_NEXT_TRACK},
            {"MEDIA_PREVIOUS_TRACK", Key::MEDIA_PREVIOUS_TRACK},
            {"MEDIA_STOP", Key::MEDIA_STOP},
            {"MEDIA_EJECT", Key::MEDIA_EJECT},
            {"MEDIA_PLAY_PAUSE", Key::MEDIA_PLAY_PAUSE},
            {"MEDIA_SELECT", Key::MEDIA_SELECT},
            {"AC_NEW", Key::AC_NEW},
            {"AC_OPEN", Key::AC_OPEN},
            {"AC_CLOSE", Key::AC_CLOSE},
            {"AC_EXIT", Key::AC_EXIT},
            {"AC_SAVE", Key::AC_SAVE},
            {"AC_PRINT", Key::AC_PRINT},
            {"AC_PROPERTIES", Key::AC_PROPERTIES},
            {"AC_SEARCH", Key::AC_SEARCH},
            {"AC_HOME", Key::AC_HOME},
            {"AC_BACK", Key::AC_BACK},
            {"AC_FORWARD", Key::AC_FORWARD},
            {"AC_STOP", Key::AC_STOP},
            {"AC_REFRESH", Key::AC_REFRESH},
            {"AC_BOOKMARKS", Key::AC_BOOKMARKS},
            {"SOFTLEFT", Key::SOFTLEFT},
            {"SOFTRIGHT", Key::SOFTRIGHT},
            {"CALL", Key::CALL},
            {"ENDCALL", Key::ENDCALL}
        };

        sol::table keyCodeTable = lua.create_table();
        for (const auto& keyCode : keyCodes) {
            keyCodeTable[keyCode.first] = keyCode.second;
        }
        inputTable["keycode"] = keyCodeTable;
    }

    void BindMouseCodesToLua(sol::state& lua, sol::table& inputTable)
    {
        std::vector<std::pair<std::string, MouseCode>> mouseCodes = {
            {"LEFT", Mouse::BUTTON_LEFT},
            {"MIDDLE", Mouse::BUTTON_MIDDLE},
            {"RIGHT", Mouse::BUTTON_RIGHT},
            {"X1", Mouse::BUTTON_X1},
            {"X2", Mouse::BUTTON_X2}
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
            return Input::IsKeyPressed(key);
        });

        inputTable.set_function("is_mouse_button_pressed", [](MouseCode button) {
            return Input::IsMouseButtonPressed(button);
        });

        inputTable.set_function("get_mouse_position", []() {
            glm::vec2 mousePosition = Input::GetMousePosition();
            return std::make_tuple(mousePosition.x, mousePosition.y);
        });

        luaState["input"] = inputTable;
        # pragma endregion

        # pragma region Bind Timer Functions
        # pragma endregion

        #pragma region Bind Entity Functions

        luaState.new_usertype<Entity>("Entity",
        sol::constructors<Entity(), Entity(entt::entity, Scene*)>(),

        "AddComponent", [](Entity& self, const std::string& componentName) {
            if (componentName == "TagComponent") {
                self.AddComponent<TagComponent>();
            } else if (componentName == "TransformComponent") {
                self.AddComponent<TransformComponent>();
            } else {
                throw std::runtime_error("Unknown component type");
            }
        },

        "GetComponent", [this](Entity& self, const std::string& componentName) -> sol::object {
            if (componentName == "TagComponent") {
                return sol::make_object(luaState, self.GetComponent<TagComponent>());
            } else if (componentName == "TransformComponent") {
                return sol::make_object(luaState, self.GetComponent<TransformComponent>());
            } else {
                throw std::runtime_error("Unknown component type");
            }
        },

        "HasComponent", [](Entity& self, const std::string& componentName) -> bool {
            if (componentName == "TagComponent") {
                return self.HasComponent<TagComponent>();
            } else if (componentName == "TransformComponent") {
                return self.HasComponent<TransformComponent>();
            } else {
                throw std::runtime_error("Unknown component type");
            }
        },

        "RemoveComponent", [](Entity& self, const std::string& componentName) {
            if (componentName == "TagComponent") {
                self.RemoveComponent<TagComponent>();
            } else if (componentName == "TransformComponent") {
                self.RemoveComponent<TransformComponent>();
            } else {
                throw std::runtime_error("Unknown component type");
            }
        },

        "SetParent", &Entity::SetParent,
        "IsValid", [](Entity& self) { return static_cast<bool>(self); }
    );

        #pragma endregion

        # pragma region Bind Components Functions
        luaState.new_usertype<TagComponent>("tag_component",
            sol::constructors<TagComponent(), TagComponent(const std::string&)>(),
            "tag", &TagComponent::Tag
        );

        luaState.new_usertype<TransformComponent>("transform_component",
            sol::constructors<TransformComponent(), TransformComponent(const glm::vec3&)>(),
            "position", &TransformComponent::Position,
            "rotation", &TransformComponent::Rotation,
            "scale", &TransformComponent::Scale,
            "get_local_transform", &TransformComponent::GetLocalTransform,
            "set_local_transform", &TransformComponent::SetLocalTransform,
            "get_world_transform", &TransformComponent::GetWorldTransform,
            "set_world_transform", &TransformComponent::SetWorldTransform
        );

        luaState.new_usertype<CameraComponent>("camera_component",
            sol::constructors<CameraComponent()>(),
            "camera", &CameraComponent::Camera
        );

        luaState.new_usertype<MeshComponent>("mesh_component",
            sol::constructors<MeshComponent(), MeshComponent(Ref<Mesh>)>(),
            "mesh", &MeshComponent::mesh,
            "drawAABB", &MeshComponent::drawAABB,
            "get_mesh", &MeshComponent::GetMesh
        );

        luaState.new_usertype<MaterialComponent>("material_component",
            sol::constructors<MaterialComponent(), MaterialComponent(Ref<Material>)>(),
            "material", &MaterialComponent::material
        );

        luaState.new_usertype<LightComponent>("light_component",
            sol::constructors<LightComponent()>(),
            "color", &LightComponent::Color,
            "direction", &LightComponent::Direction,
            "position", &LightComponent::Position,
            "range", &LightComponent::Range,
            "attenuation", &LightComponent::Attenuation,
            "intensity", &LightComponent::Intensity,
            "angle", &LightComponent::Angle,
            "type", &LightComponent::type
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

    void LuaBackend::RegisterVariable(const std::string& name, void* variable)
    {
        luaState[name] = variable;
    }

    // This function will check all the variables from the script and map them to a cpp map so we can expose them in the editor
    std::vector<LuaVariable> LuaBackend::MapVariables(const std::string& scriptPath) {
        std::vector<LuaVariable> variables;
        std::ifstream scriptFile(scriptPath);
        std::string script((std::istreambuf_iterator<char>(scriptFile)), std::istreambuf_iterator<char>());

        std::regex exportRegex(R"(--\[\[export\]\]\s+(\w+)\s*=\s*(.+))"); // --[[export]] variable = value
        std::smatch match;
        std::string::const_iterator searchStart(script.cbegin());

        while (std::regex_search(searchStart, script.cend(), match, exportRegex)) {
            LuaVariable variable;
            variable.name = match[1];
            variable.value = match[2];

            // Store the variable in the vector
            variables.push_back(variable);

            // Move to the next match
            searchStart = match.suffix().first;
        }

        return variables;
    }

} // namespace Coffee