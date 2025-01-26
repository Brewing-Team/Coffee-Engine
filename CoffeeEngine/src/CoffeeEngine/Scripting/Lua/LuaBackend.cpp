#include "LuaBackend.h"

#include "CoffeeEngine/Core/Input.h"
#include "CoffeeEngine/Core/KeyCodes.h"
#include "CoffeeEngine/Core/Log.h"
#include "CoffeeEngine/Core/MouseCodes.h"
#include <fstream>
#include <lua.h>
#include <regex>
#include "CoffeeEngine/Scene/Components.h"
#include "CoffeeEngine/Scene/Entity.h"
#include "CoffeeEngine/Scripting/Lua/LuaScript.h"

#define SOL_PRINT_ERRORS 1

namespace Coffee {

    sol::state LuaBackend::luaState;

    void BindKeyCodesToLua(sol::state& lua, sol::table& inputTable)
    {
        std::vector<std::pair<std::string, KeyCode>> keyCodes = {
            {"Unknown", Key::Unknown},
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
            {"Return", Key::Return},
            {"Escape", Key::Escape},
            {"Backspace", Key::Backspace},
            {"Tab", Key::Tab},
            {"Space", Key::Space},
            {"Minus", Key::Minus},
            {"Equals", Key::Equals},
            {"LeftBracket", Key::LeftBracket},
            {"RightBracket", Key::RightBracket},
            {"Backslash", Key::Backslash},
            {"NonUsHash", Key::NonUsHash},
            {"Semicolon", Key::Semicolon},
            {"Apostrophe", Key::Apostrophe},
            {"Grave", Key::Grave},
            {"Comma", Key::Comma},
            {"Period", Key::Period},
            {"Slash", Key::Slash},
            {"CapsLock", Key::CapsLock},
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
            {"PrintScreen", Key::PrintScreen},
            {"ScrollLock", Key::ScrollLock},
            {"Pause", Key::Pause},
            {"Insert", Key::Insert},
            {"Home", Key::Home},
            {"PageUp", Key::PageUp},
            {"Delete", Key::Delete},
            {"End", Key::End},
            {"PageDown", Key::PageDown},
            {"Right", Key::Right},
            {"Left", Key::Left},
            {"Down", Key::Down},
            {"Up", Key::Up},
            {"NumLockClear", Key::NumLockClear},
            {"KpDivide", Key::KpDivide},
            {"KpMultiply", Key::KpMultiply},
            {"KpMinus", Key::KpMinus},
            {"KpPlus", Key::KpPlus},
            {"KpEnter", Key::KpEnter},
            {"Kp1", Key::Kp1},
            {"Kp2", Key::Kp2},
            {"Kp3", Key::Kp3},
            {"Kp4", Key::Kp4},
            {"Kp5", Key::Kp5},
            {"Kp6", Key::Kp6},
            {"Kp7", Key::Kp7},
            {"Kp8", Key::Kp8},
            {"Kp9", Key::Kp9},
            {"Kp0", Key::Kp0},
            {"KpPeriod", Key::KpPeriod},
            {"NonUsBackslash", Key::NonUsBackslash},
            {"Application", Key::Application},
            {"Power", Key::Power},
            {"KpEquals", Key::KpEquals},
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
            {"Execute", Key::Execute},
            {"Help", Key::Help},
            {"Menu", Key::Menu},
            {"Select", Key::Select},
            {"Stop", Key::Stop},
            {"Again", Key::Again},
            {"Undo", Key::Undo},
            {"Cut", Key::Cut},
            {"Copy", Key::Copy},
            {"Paste", Key::Paste},
            {"Find", Key::Find},
            {"Mute", Key::Mute},
            {"VolumeUp", Key::VolumeUp},
            {"VolumeDown", Key::VolumeDown},
            {"KpComma", Key::KpComma},
            {"KpEqualsAs400", Key::KpEqualsAs400},
            {"International1", Key::International1},
            {"International2", Key::International2},
            {"International3", Key::International3},
            {"International4", Key::International4},
            {"International5", Key::International5},
            {"International6", Key::International6},
            {"International7", Key::International7},
            {"International8", Key::International8},
            {"International9", Key::International9},
            {"Lang1", Key::Lang1},
            {"Lang2", Key::Lang2},
            {"Lang3", Key::Lang3},
            {"Lang4", Key::Lang4},
            {"Lang5", Key::Lang5},
            {"Lang6", Key::Lang6},
            {"Lang7", Key::Lang7},
            {"Lang8", Key::Lang8},
            {"Lang9", Key::Lang9},
            {"AltErase", Key::AltErase},
            {"SysReq", Key::SysReq},
            {"Cancel", Key::Cancel},
            {"Clear", Key::Clear},
            {"Prior", Key::Prior},
            {"Return2", Key::Return2},
            {"Separator", Key::Separator},
            {"Out", Key::Out},
            {"Oper", Key::Oper},
            {"ClearAgain", Key::ClearAgain},
            {"CrSel", Key::CrSel},
            {"ExSel", Key::ExSel},
            {"Kp00", Key::Kp00},
            {"Kp000", Key::Kp000},
            {"ThousandsSeparator", Key::ThousandsSeparator},
            {"DecimalSeparator", Key::DecimalSeparator},
            {"CurrencyUnit", Key::CurrencyUnit},
            {"CurrencySubUnit", Key::CurrencySubUnit},
            {"KpLeftParen", Key::KpLeftParen},
            {"KpRightParen", Key::KpRightParen},
            {"KpLeftBrace", Key::KpLeftBrace},
            {"KpRightBrace", Key::KpRightBrace},
            {"KpTab", Key::KpTab},
            {"KpBackspace", Key::KpBackspace},
            {"KpA", Key::KpA},
            {"KpB", Key::KpB},
            {"KpC", Key::KpC},
            {"KpD", Key::KpD},
            {"KpE", Key::KpE},
            {"KpF", Key::KpF},
            {"KpXor", Key::KpXor},
            {"KpPower", Key::KpPower},
            {"KpPercent", Key::KpPercent},
            {"KpLess", Key::KpLess},
            {"KpGreater", Key::KpGreater},
            {"KpAmpersand", Key::KpAmpersand},
            {"KpDblAmpersand", Key::KpDblAmpersand},
            {"KpVerticalBar", Key::KpVerticalBar},
            {"KpDblVerticalBar", Key::KpDblVerticalBar},
            {"KpColon", Key::KpColon},
            {"KpHash", Key::KpHash},
            {"KpSpace", Key::KpSpace},
            {"KpAt", Key::KpAt},
            {"KpExclam", Key::KpExclam},
            {"KpMemStore", Key::KpMemStore},
            {"KpMemRecall", Key::KpMemRecall},
            {"KpMemClear", Key::KpMemClear},
            {"KpMemAdd", Key::KpMemAdd},
            {"KpMemSubtract", Key::KpMemSubtract},
            {"KpMemMultiply", Key::KpMemMultiply},
            {"KpMemDivide", Key::KpMemDivide},
            {"KpPlusMinus", Key::KpPlusMinus},
            {"KpClear", Key::KpClear},
            {"KpClearEntry", Key::KpClearEntry},
            {"KpBinary", Key::KpBinary},
            {"KpOctal", Key::KpOctal},
            {"KpDecimal", Key::KpDecimal},
            {"KpHexadecimal", Key::KpHexadecimal},
            {"LCtrl", Key::LCtrl},
            {"LShift", Key::LShift},
            {"LAlt", Key::LAlt},
            {"LGui", Key::LGui},
            {"RCtrl", Key::RCtrl},
            {"RShift", Key::RShift},
            {"RAlt", Key::RAlt},
            {"RGui", Key::RGui},
            {"Mode", Key::Mode},
            {"Sleep", Key::Sleep},
            {"Wake", Key::Wake},
            {"ChannelIncrement", Key::ChannelIncrement},
            {"ChannelDecrement", Key::ChannelDecrement},
            {"MediaPlay", Key::MediaPlay},
            {"MediaPause", Key::MediaPause},
            {"MediaRecord", Key::MediaRecord},
            {"MediaFastForward", Key::MediaFastForward},
            {"MediaRewind", Key::MediaRewind},
            {"MediaNextTrack", Key::MediaNextTrack},
            {"MediaPreviousTrack", Key::MediaPreviousTrack},
            {"MediaStop", Key::MediaStop},
            {"MediaEject", Key::MediaEject},
            {"MediaPlayPause", Key::MediaPlayPause},
            {"MediaSelect", Key::MediaSelect},
            {"AcNew", Key::AcNew},
            {"AcOpen", Key::AcOpen},
            {"AcClose", Key::AcClose},
            {"AcExit", Key::AcExit},
            {"AcSave", Key::AcSave},
            {"AcPrint", Key::AcPrint},
            {"AcProperties", Key::AcProperties},
            {"AcSearch", Key::AcSearch},
            {"AcHome", Key::AcHome},
            {"AcBack", Key::AcBack},
            {"AcForward", Key::AcForward},
            {"AcStop", Key::AcStop},
            {"AcRefresh", Key::AcRefresh},
            {"AcBookmarks", Key::AcBookmarks},
            {"SoftLeft", Key::SoftLeft},
            {"SoftRight", Key::SoftRight},
            {"Call", Key::Call},
            {"EndCall", Key::EndCall}
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
            {"Left", Mouse::ButtonLeft},
            {"Middle", Mouse::ButtonMiddle},
            {"Right", Mouse::ButtonRight},
            {"X1", Mouse::ButtonX1},
            {"X2", Mouse::ButtonX2}
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

        luaState["Input"] = inputTable;
        # pragma endregion

        # pragma region Bind Timer Functions
        # pragma endregion

        # pragma region Bind GLM Functions
        luaState.new_usertype<glm::vec2>("Vector2",
            sol::constructors<glm::vec2(), glm::vec2(float), glm::vec2(float, float)>(),
            "x", &glm::vec2::x,
            "y", &glm::vec2::y,
            "normalize", [](const glm::vec2& a) { return glm::normalize(a); },
            "length", [](const glm::vec2& a) { return glm::length(a); },
            "length_squared", [](const glm::vec2& a) { return glm::length2(a); },
            "distance_to", [](const glm::vec2& a, const glm::vec2& b) { return glm::distance(a, b); },
            "distance_squared_to", [](const glm::vec2& a, const glm::vec2& b) { return glm::distance2(a, b); },
            "lerp", [](const glm::vec2& a, const glm::vec2& b, float t) { return glm::mix(a, b, t); },
            "dot", [](const glm::vec2& a, const glm::vec2& b) { return glm::dot(a, b); },
            "angle_to", [](const glm::vec2& a, const glm::vec2& b) { return glm::degrees(glm::acos(glm::dot(glm::normalize(a), glm::normalize(b)))); },
            "max", [](const glm::vec2& a, const glm::vec2& b) { return glm::max(a, b); },
            "min", [](const glm::vec2& a, const glm::vec2& b) { return glm::min(a, b); },
            "abs", [](const glm::vec2& a) { return glm::abs(a); }
            //TODO: Add more functions
        );

        luaState.new_usertype<glm::vec3>("Vector3",
            sol::constructors<glm::vec3(), glm::vec3(float), glm::vec3(float, float, float)>(),
            "x", &glm::vec3::x,
            "y", &glm::vec3::y,
            "z", &glm::vec3::z,
            "cross", [](const glm::vec3& a, const glm::vec3& b) { return glm::cross(a, b); },
            "dot", [](const glm::vec3& a, const glm::vec3& b) { return glm::dot(a, b); },
            "normalize", [](const glm::vec3& a) { return glm::normalize(a); },
            "length", [](const glm::vec3& a) { return glm::length(a); },
            "length_squared", [](const glm::vec3& a) { return glm::length2(a); },
            "distance_to", [](const glm::vec3& a, const glm::vec3& b) { return glm::distance(a, b); },
            "distance_squared_to", [](const glm::vec3& a, const glm::vec3& b) { return glm::distance2(a, b); },
            "lerp", [](const glm::vec3& a, const glm::vec3& b, float t) { return glm::mix(a, b, t); },
            "dot", [](const glm::vec3& a, const glm::vec3& b) { return glm::dot(a, b); },
            "angle_to", [](const glm::vec3& a, const glm::vec3& b) { return glm::degrees(glm::acos(glm::dot(glm::normalize(a), glm::normalize(b)))); },
            "max", [](const glm::vec3& a, const glm::vec3& b) { return glm::max(a, b); },
            "min", [](const glm::vec3& a, const glm::vec3& b) { return glm::min(a, b); },
            "abs", [](const glm::vec3& a) { return glm::abs(a); }
            //TODO: Add more functions
        );

        luaState.new_usertype<glm::vec4>("Vector4",
            sol::constructors<glm::vec4(), glm::vec4(float), glm::vec4(float, float, float, float)>(),
            "x", &glm::vec4::x,
            "y", &glm::vec4::y,
            "z", &glm::vec4::z,
            "w", &glm::vec4::w,
            "normalize", [](const glm::vec4& a) { return glm::normalize(a); },
            "length", [](const glm::vec4& a) { return glm::length(a); },
            "length_squared", [](const glm::vec4& a) { return glm::length2(a); },
            "distance_to", [](const glm::vec4& a, const glm::vec4& b) { return glm::distance(a, b); },
            "distance_squared_to", [](const glm::vec4& a, const glm::vec4& b) { return glm::distance2(a, b); },
            "lerp", [](const glm::vec4& a, const glm::vec4& b, float t) { return glm::mix(a, b, t); },
            "dot", [](const glm::vec4& a, const glm::vec4& b) { return glm::dot(a, b); },
            "angle_to", [](const glm::vec4& a, const glm::vec4& b) { return glm::degrees(glm::acos(glm::dot(glm::normalize(a), glm::normalize(b)))); },
            "max", [](const glm::vec4& a, const glm::vec4& b) { return glm::max(a, b); },
            "min", [](const glm::vec4& a, const glm::vec4& b) { return glm::min(a, b); },
            "abs", [](const glm::vec4& a) { return glm::abs(a); }
            //TODO: Add more functions
        );

        luaState.new_usertype<glm::mat4>("Mat4",
            sol::constructors<glm::mat4(), glm::mat4(float)>(),
            "identity", []() { return glm::mat4(1.0f); },
            "inverse", [](const glm::mat4& mat) { return glm::inverse(mat); },
            "transpose", [](const glm::mat4& mat) { return glm::transpose(mat); },
            "translate", [](const glm::mat4& mat, const glm::vec3& vec) { return glm::translate(mat, vec); },
            "rotate", [](const glm::mat4& mat, float angle, const glm::vec3& axis) { return glm::rotate(mat, angle, axis); },
            "scale", [](const glm::mat4& mat, const glm::vec3& vec) { return glm::scale(mat, vec); },
            "perspective", [](float fovy, float aspect, float near, float far) { return glm::perspective(fovy, aspect, near, far); },
            "ortho", [](float left, float right, float bottom, float top, float zNear, float zFar) { return glm::ortho(left, right, bottom, top, zNear, zFar); }
        );

        luaState.new_usertype<glm::quat>("Quaternion",
            sol::constructors<glm::quat(), glm::quat(float, float, float, float), glm::quat(const glm::vec3&), glm::quat(float, const glm::vec3&)>(),
            "x", &glm::quat::x,
            "y", &glm::quat::y,
            "z", &glm::quat::z,
            "w", &glm::quat::w,
            "from_euler", [](const glm::vec3& euler) { return glm::quat(glm::radians(euler)); },
            "to_euler_angles", [](const glm::quat& q) { return glm::eulerAngles(q); },
            "toMat4", [](const glm::quat& q) { return glm::toMat4(q); },
            "normalize", [](const glm::quat& q) { return glm::normalize(q); },
            "slerp", [](const glm::quat& a, const glm::quat& b, float t) { return glm::slerp(a, b, t); }
        );
        # pragma endregion

        #pragma endregion

        #pragma region Bind Entity Functions

        luaState.new_usertype<Entity>("Entity",
            sol::constructors<Entity(), Entity(entt::entity, Scene*)>(),
            "add_component", [](Entity* self, const std::string& componentName) {
                if (componentName == "TagComponent") {
                    self->AddComponent<TagComponent>();
                } else if (componentName == "TransformComponent") {
                    self->AddComponent<TransformComponent>();
                } else if (componentName == "CameraComponent") {
                    self->AddComponent<CameraComponent>();
                } else if (componentName == "MeshComponent") {
                    self->AddComponent<MeshComponent>();
                } else if (componentName == "MaterialComponent") {
                    self->AddComponent<MaterialComponent>();
                } else if (componentName == "LightComponent") {
                    self->AddComponent<LightComponent>();
                }
            },
            "get_component", [this](Entity* self, const std::string& componentName) -> sol::object {
                if (componentName == "TagComponent") {
                    return sol::make_object(luaState, self->GetComponent<TagComponent>());
                } else if (componentName == "TransformComponent") {
                    return sol::make_object(luaState, std::ref(self->GetComponent<TransformComponent>()));
                } else if (componentName == "CameraComponent") {
                    return sol::make_object(luaState, std::ref(self->GetComponent<CameraComponent>()));
                } else if (componentName == "MeshComponent") {
                    return sol::make_object(luaState, std::ref(self->GetComponent<MeshComponent>()));
                } else if (componentName == "MaterialComponent") {
                    return sol::make_object(luaState, std::ref(self->GetComponent<MaterialComponent>()));
                } else if (componentName == "LightComponent") {
                    return sol::make_object(luaState, std::ref(self->GetComponent<LightComponent>()));
                }
                return sol::nil;
            },
            "has_component", [](Entity* self, const std::string& componentName) -> bool {
                if (componentName == "TagComponent") {
                    return self->HasComponent<TagComponent>();
                } else if (componentName == "TransformComponent") {
                    return self->HasComponent<TransformComponent>();
                } else if (componentName == "CameraComponent") {
                    return self->HasComponent<CameraComponent>();
                } else if (componentName == "MeshComponent") {
                    return self->HasComponent<MeshComponent>();
                } else if (componentName == "MaterialComponent") {
                    return self->HasComponent<MaterialComponent>();
                } else if (componentName == "LightComponent") {
                    return self->HasComponent<LightComponent>();
                }
                return false;
            },
            "remove_component", [](Entity* self, const std::string& componentName) {
                if (componentName == "TagComponent") {
                    self->RemoveComponent<TagComponent>();
                } else if (componentName == "TransformComponent") {
                    self->RemoveComponent<TransformComponent>();
                } else if (componentName == "CameraComponent") {
                    self->RemoveComponent<CameraComponent>();
                } else if (componentName == "MeshComponent") {
                    self->RemoveComponent<MeshComponent>();
                } else if (componentName == "MaterialComponent") {
                    self->RemoveComponent<MaterialComponent>();
                } else if (componentName == "LightComponent") {
                    self->RemoveComponent<LightComponent>();
                }
            },
            "set_parent", &Entity::SetParent,
            "get_parent", &Entity::GetParent,
            "get_next_sibling", &Entity::GetNextSibling,
            "get_prev_sibling", &Entity::GetPrevSibling,
            "get_child", &Entity::GetChild,
            "get_children", &Entity::GetChildren,
            "is_valid", [](Entity* self) { return static_cast<bool>(*self); }
        );
        #pragma endregion

        # pragma region Bind Components Functions
        luaState.new_usertype<TagComponent>("TagComponent",
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

        # pragma region Bind Scene Functions

        luaState.new_usertype<Scene>("Scene",
            "create_entity", &Scene::CreateEntity,
            "destroy_entity", &Scene::DestroyEntity,
            "get_entity_by_name", &Scene::GetEntityByName,
            "get_all_entities", &Scene::GetAllEntities
        );

        # pragma endregion

    }

    Ref<Script> LuaBackend::CreateScript(const std::filesystem::path& path) {
        return CreateRef<LuaScript>(path);
    }

    void LuaBackend::ExecuteScript(Script& script) {
        LuaScript& luaScript = static_cast<LuaScript&>(const_cast<Script&>(script));
        try {
            luaState.script_file(luaScript.GetPath().string(), luaScript.GetEnvironment());
        } catch (const sol::error& e) {
            COFFEE_CORE_ERROR("Lua: {0}", e.what());
        }
        script.ParseScript();
    }

} // namespace Coffee