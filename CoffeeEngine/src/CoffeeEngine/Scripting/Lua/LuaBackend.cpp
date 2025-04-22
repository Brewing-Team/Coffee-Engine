#include "LuaBackend.h"

#include "Bindings/LuaInput.h"
#include "Bindings/LuaLog.h"
#include "Bindings/LuaMath.h"
#include "Bindings/LuaPhysics.h"
#include "Bindings/LuaPrefab.h"
#include "Bindings/LuaScene.h"

#include "CoffeeEngine/Core/Log.h"

#include "CoffeeEngine/Core/Stopwatch.h"
#include "CoffeeEngine/Core/Timer.h"

#include "CoffeeEngine/Scene/Components.h"
#include "CoffeeEngine/Scene/Entity.h"
#include "CoffeeEngine/Scene/SceneManager.h"
#include "CoffeeEngine/Scripting/GameSaver.h"
#include "CoffeeEngine/Scripting/Lua/LuaScript.h"
#include <fstream>
#include <lua.h>
#include <regex>

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <sol/property.hpp>
#include <sol/types.hpp>

#define SOL_PRINT_ERRORS 1

namespace Coffee {

    void LuaBackend::Initialize() {
        luaState.open_libraries(sol::lib::base, sol::lib::math, sol::lib::string, sol::lib::table, sol::lib::package, sol::lib::coroutine);

        dafaultPackagePath = luaState["package"]["path"];

        RegisterLogBindings(luaState);
        
        RegisterInputBindings(luaState);

        # pragma region Bind Timer and Stopwatch Functions

        // Bind Stopwatch class
        luaState.new_usertype<Stopwatch>("Stopwatch",
            sol::constructors<Stopwatch()>(),
            "start", &Stopwatch::Start,
            "stop", &Stopwatch::Stop,
            "reset", &Stopwatch::Reset,
            "get_elapsed_time", &Stopwatch::GetElapsedTime,
            "get_precise_elapsed_time", &Stopwatch::GetPreciseElapsedTime
        );

        // Bind Timer class
        luaState.new_usertype<Timer>("Timer",
            sol::constructors<Timer(), Timer(double, bool, bool, Timer::TimerCallback)>(),
            "start", &Timer::Start,
            "stop", &Timer::Stop,
            "set_wait_time", &Timer::setWaitTime,
            "get_wait_time", &Timer::getWaitTime,
            "set_one_shot", &Timer::setOneShot,
            "is_one_shot", &Timer::isOneShot,
            "set_auto_start", &Timer::setAutoStart,
            "is_auto_start", &Timer::isAutoStart,
            "set_paused", &Timer::setPaused,
            "is_paused", &Timer::isPaused,
            "get_time_left", &Timer::GetTimeLeft,
            "set_callback", [](Timer& timer, const sol::protected_function& callback) {
                timer.SetCallback([callback] {
                    if (const auto result = callback(); !result.valid()) {
                        const sol::error err = result;
                        COFFEE_CORE_ERROR("Lua: Timer callback error: {0}", err.what());
                    }
                });
            }
        );

        // Helper function to create a timer
        luaState.set_function("create_timer", [](double waitTime, bool autoStart, bool oneShot, sol::protected_function callback) {
            return Timer(waitTime, autoStart, oneShot, [callback]() {
                auto result = callback();
                if (!result.valid()) {
                    sol::error err = result;
                    COFFEE_CORE_ERROR("Lua: Timer callback error: {0}", err.what());
                }
            });
        });

        # pragma endregion

        RegisterMathBindings(luaState);

        #pragma region Bind Entity Functions
        luaState.new_usertype<Entity>("Entity",
            sol::constructors<Entity(), Entity(entt::entity, Scene*)>(),
            "get_component", [this](Entity* self, const std::string& componentName) -> sol::object {
                if (componentName == "TagComponent") {
                    if (auto* component = self->TryGetComponent<TagComponent>())
                        return sol::make_object(luaState, std::ref(*component));

                    COFFEE_CORE_ERROR("Lua: Entity does not have a TagComponent");
                    return sol::nil;
                } else if (componentName == "TransformComponent") {
                    if (auto* component = self->TryGetComponent<TransformComponent>())
                        return sol::make_object(luaState, std::ref(*component));

                    COFFEE_CORE_ERROR("Lua: Entity does not have a TransformComponent");
                    return sol::nil;
                } else if (componentName == "CameraComponent") {
                    if (auto* component = self->TryGetComponent<CameraComponent>())
                        return sol::make_object(luaState, std::ref(*component));


                    COFFEE_CORE_ERROR("Lua: Entity does not have a CameraComponent");
                    return sol::nil;
                } else if (componentName == "MeshComponent") {
                    if (auto* component = self->TryGetComponent<MeshComponent>())
                        return sol::make_object(luaState, std::ref(*component));

                    COFFEE_CORE_ERROR("Lua: Entity does not have a MeshComponent");
                    return sol::nil;
                } else if (componentName == "MaterialComponent") {
                    if (auto* component = self->TryGetComponent<MaterialComponent>())
                        return sol::make_object(luaState, std::ref(*component));
                    COFFEE_CORE_ERROR("Lua: Entity does not have a MaterialComponent");
                    return sol::nil;
                } else if (componentName == "LightComponent") {
                    if (auto* component = self->TryGetComponent<LightComponent>())
                        return sol::make_object(luaState, std::ref(*component));

                    COFFEE_CORE_ERROR("Lua: Entity does not have a LightComponent");
                    return sol::nil;
                } else if (componentName == "ScriptComponent") {
                    if (auto* component = self->TryGetComponent<ScriptComponent>())
                        return sol::make_object(luaState, std::ref(*component));

                    COFFEE_CORE_ERROR("Lua: Entity does not have a ScriptComponent");
                    return sol::nil;
                } else if (componentName == "ParticlesSystemComponent") {
                    if (auto* component = self->TryGetComponent<ParticlesSystemComponent>())
                        return sol::make_object(luaState, std::ref(*component));

                    COFFEE_CORE_ERROR("Lua: Entity does not have a ParticlesSystemComponent");
                    return sol::nil;
                } else if (componentName == "NavigationAgentComponent") {
                    if (auto* component = self->TryGetComponent<NavigationAgentComponent>())
                        return sol::make_object(luaState, std::ref(*component));

                    COFFEE_CORE_ERROR("Lua: Entity does not have a NavigationAgentComponent");
                    return sol::nil;
                } else if (componentName == "RigidbodyComponent") {
                    if (auto* component = self->TryGetComponent<RigidbodyComponent>())
                        return sol::make_object(luaState, std::ref(*component));

                    COFFEE_CORE_ERROR("Lua: Entity does not have a RigidbodyComponent");
                    return sol::nil;
                } else if (componentName == "AudioSourceComponent") {
                    if (auto* component = self->TryGetComponent<AudioSourceComponent>())
                        return sol::make_object(luaState, std::ref(*component));

                    COFFEE_CORE_ERROR("Lua: Entity does not have a AudioSourceComponent");
                    return sol::nil;
                } else if (componentName == "AnimatorComponent") {
                    if (auto* component = self->TryGetComponent<AnimatorComponent>())
                        return sol::make_object(luaState, std::ref(*component));

                    COFFEE_CORE_ERROR("Lua: Entity does not have a AnimatorComponent");
                    return sol::nil;
                } else if (componentName == "UIImageComponent") {
                    if (auto* component = self->TryGetComponent<UIImageComponent>())
                        return sol::make_object(luaState, std::ref(*component));

                    COFFEE_CORE_ERROR("Lua: Entity does not have a UIImageComponent");
                    return sol::nil;
                } else if (componentName == "UITextComponent") {
                    if (auto* component = self->TryGetComponent<UITextComponent>())
                        return sol::make_object(luaState, std::ref(*component));

                    COFFEE_CORE_ERROR("Lua: Entity does not have a UITextComponent");
                    return sol::nil;
                } else if (componentName == "UIToggleComponent") {
                    if (auto* component = self->TryGetComponent<UIToggleComponent>())
                        return sol::make_object(luaState, std::ref(*component));

                    COFFEE_CORE_ERROR("Lua: Entity does not have a UIToggleComponent");
                    return sol::nil;
                } else if (componentName == "UIButtonComponent") {
                    if (auto* component = self->TryGetComponent<UIButtonComponent>())
                        return sol::make_object(luaState, std::ref(*component));

                    COFFEE_CORE_ERROR("Lua: Entity does not have a UIButtonComponent");
                    return sol::nil;
                } else if (componentName == "UISliderComponent") {
                    if (auto* component = self->TryGetComponent<UISliderComponent>())
                        return sol::make_object(luaState, std::ref(*component));

                    COFFEE_CORE_ERROR("Lua: Entity does not have a UISliderComponent");
                    return sol::nil;
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
                } else if (componentName == "ScriptComponent") {
                    return self->HasComponent<ScriptComponent>();
                } else if (componentName == "ParticlesSystemComponent") {
                    return self->HasComponent<ParticlesSystemComponent>();
                } else if (componentName == "NavigationAgentComponent") {
                    return self->HasComponent<NavigationAgentComponent>();
                } else if (componentName == "RigidbodyComponent") {
                    return self->HasComponent<RigidbodyComponent>();
                } else if (componentName == "AnimatorComponent") {
                    return self->HasComponent<AnimatorComponent>();
                } else if (componentName == "AudioSourceComponent") {
                    return self->HasComponent<AudioSourceComponent>();
                } else if (componentName == "UIImageComponent") {
                    return self->HasComponent<UIImageComponent>();
                } else if (componentName == "UITextComponent") {
                    return self->HasComponent<UITextComponent>();
                } else if (componentName == "UIToggleComponent") {
                    return self->HasComponent<UIToggleComponent>();
                } else if (componentName == "UIButtonComponent") {
                    return self->HasComponent<UIButtonComponent>();
                } else if (componentName == "UISliderComponent") {
                    return self->HasComponent<UISliderComponent>();
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
                } else if (componentName == "ScriptComponent") {
                    self->RemoveComponent<ScriptComponent>();
                } else if (componentName == "ParticlesSystemComponent") {
                    self->RemoveComponent<ParticlesSystemComponent>();
                } else if (componentName == "RigidbodyComponent") {
                    self->RemoveComponent<RigidbodyComponent>();
                } else if (componentName == "AudioSourceComponent") {
                    self->RemoveComponent<AudioSourceComponent>();
                }
            },
            "set_parent", &Entity::SetParent,
            "get_parent", &Entity::GetParent,
            "get_next_sibling", &Entity::GetNextSibling,
            "get_prev_sibling", &Entity::GetPrevSibling,
            "get_child", &Entity::GetChild,
            "get_children", &Entity::GetChildren,
            "is_valid", [](Entity* self) { return static_cast<bool>(*self); },
            "is_active", &Entity::IsActive,
            "set_active", &Entity::SetActive
        );
        #pragma endregion

        # pragma region Bind Components Functions
        luaState.new_usertype<TagComponent>("TagComponent",
            sol::constructors<TagComponent(), TagComponent(const std::string&)>(),
            "tag", &TagComponent::Tag
        );

        luaState.new_usertype<TransformComponent>("TransformComponent",
            sol::constructors<TransformComponent(), TransformComponent(const glm::vec3&)>(),
            "position", sol::property(&TransformComponent::GetLocalPosition, &TransformComponent::SetLocalPosition),
            "rotation", sol::property(&TransformComponent::GetLocalRotation, &TransformComponent::SetLocalRotation),
            "scale", sol::property(&TransformComponent::GetLocalScale, &TransformComponent::SetLocalScale),
            "get_local_transform", &TransformComponent::GetLocalTransform,
            "set_local_transform", &TransformComponent::SetLocalTransform,
            "get_world_transform", &TransformComponent::GetWorldTransform,
            "set_world_transform", &TransformComponent::SetWorldTransform
        );

        luaState.new_usertype<CameraComponent>("CameraComponent",
            sol::constructors<CameraComponent()>(),
            "camera", &CameraComponent::Camera
        );

        luaState.new_usertype<MeshComponent>("MeshComponent",
            sol::constructors<MeshComponent(), MeshComponent(Ref<Mesh>)>(),
            "mesh", &MeshComponent::mesh,
            "drawAABB", &MeshComponent::drawAABB,
            "get_mesh", &MeshComponent::GetMesh
        );

        luaState.new_usertype<MaterialComponent>("MaterialComponent",
            sol::constructors<MaterialComponent(), MaterialComponent(Ref<Material>)>(),
            "material", &MaterialComponent::material
        );

        luaState.new_usertype<LightComponent>("LightComponent",
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

        luaState.new_usertype<NavigationAgentComponent>("NavigationAgentComponent",
            sol::constructors<NavigationAgentComponent()>(),
            "path", &NavigationAgentComponent::Path,
            "find_path", &NavigationAgentComponent::FindPath
        );

        luaState.new_usertype<ScriptComponent>("ScriptComponent",
            sol::constructors<ScriptComponent(), ScriptComponent(const std::filesystem::path& path, ScriptingLanguage language)>(),
            sol::meta_function::index, [](ScriptComponent& self, const std::string& key) {
                return std::dynamic_pointer_cast<LuaScript>(self.script)->GetVariable<sol::object>(key);
            },
            sol::meta_function::new_index, [](ScriptComponent& self, const std::string& key, sol::object value) {
                std::dynamic_pointer_cast<LuaScript>(self.script)->SetVariable(key, value);
            },
            sol::meta_function::call, [](ScriptComponent& self, const std::string& functionName, sol::variadic_args args) {
                std::dynamic_pointer_cast<LuaScript>(self.script)->CallFunction(functionName);
            }
        );




        luaState.new_usertype<ParticlesSystemComponent>("ParticlesSystemComponent", sol::constructors<ParticlesSystemComponent()>(), 
            "emit",&ParticlesSystemComponent::Emit, 
            "set_looping",&ParticlesSystemComponent::SetLooping, 
            "get_emitter", &ParticlesSystemComponent::GetParticleEmitter
            );

        luaState.new_usertype<RigidbodyComponent>("RigidbodyComponent",
            "rb", &RigidbodyComponent::rb,
            "on_collision_enter", [](RigidbodyComponent& self, sol::protected_function fn) {
                self.callback.OnCollisionEnter([fn](CollisionInfo& info) {
                    fn(info.entityA, info.entityB);
                });
            },
            "on_collision_stay", [](RigidbodyComponent& self, sol::protected_function fn) {
                self.callback.OnCollisionStay([fn](CollisionInfo& info) {
                    fn(info.entityA, info.entityB);
                });
            },
            "on_collision_exit", [](RigidbodyComponent& self, sol::protected_function fn) {
                self.callback.OnCollisionExit([fn](CollisionInfo& info) {
                    fn(info.entityA, info.entityB);
                });
            }
        );

        luaState.new_usertype<AnimatorComponent>(
            "AnimatorComponent", sol::constructors<AnimatorComponent(), AnimatorComponent()>(),
            "set_current_animation", &AnimatorComponent::SetCurrentAnimation,
            "set_upper_animation", &AnimatorComponent::SetUpperAnimation,
            "set_lower_animation", &AnimatorComponent::SetLowerAnimation
        );

        luaState.new_usertype<AudioSourceComponent>("AudioSourceComponent",
        sol::constructors<AudioSourceComponent(), AudioSourceComponent()>(),
         "set_volume", &AudioSourceComponent::SetVolume,
         "play", &AudioSourceComponent::Play,
         "pause", &AudioSourceComponent::Stop);

        luaState.new_usertype<UIImageComponent>("UIImageComponent", sol::constructors<UIImageComponent()>(),
            "set_color", [](UIImageComponent& self, const glm::vec4& color) { self.Color = color; }
        );

        luaState.new_usertype<UITextComponent>("UITextComponent",
            "set_text", [](UITextComponent& self, const std::string& text) { self.Text = text; },
            "set_color", [](UITextComponent& self, const glm::vec4& color) { self.Color = color; },
            "kerning", &UITextComponent::Kerning,
            "line_spacing", &UITextComponent::LineSpacing,
            "font_size", &UITextComponent::FontSize
        );

        luaState.new_usertype<UIToggleComponent>("UIToggleComponent",
            "value", &UIToggleComponent::Value
        );

        luaState.new_enum<UIButtonComponent::State>("State",
        {
            {"Normal", UIButtonComponent::State::Normal},
            {"Hover", UIButtonComponent::State::Hover},
            {"Pressed", UIButtonComponent::State::Pressed},
            {"Disabled", UIButtonComponent::State::Disabled}
        });

        luaState.new_usertype<UIButtonComponent>("UIButtonComponent",
            "interactable", &UIButtonComponent::Interactable,
            "state", &UIButtonComponent::CurrentState,
            "set_normal_color", [](UIButtonComponent& self, const glm::vec4& color) { self.NormalColor = color; },
            "set_hover_color", [](UIButtonComponent& self, const glm::vec4& color) { self.HoverColor = color; },
            "set_pressed_color", [](UIButtonComponent& self, const glm::vec4& color) { self.PressedColor = color; },
            "set_disabled_color", [](UIButtonComponent& self, const glm::vec4& color) { self.DisabledColor = color; }
        );

        luaState.new_usertype<UISliderComponent>("UISliderComponent",
            "value", &UISliderComponent::Value,
            "min_value", &UISliderComponent::MinValue,
            "max_value", &UISliderComponent::MaxValue,
            "handle_scale", &UISliderComponent::HandleScale
        );

        luaState.set_function("save_progress", [](const std::string& key, const sol::object& value) {
            if (value.is<int>()) {
                GameSaver::GetInstance().SaveVariable(key, value.as<int>());
            } else if (value.is<float>()) {
                GameSaver::GetInstance().SaveVariable(key, value.as<float>());
            } else if (value.is<bool>()) {
                GameSaver::GetInstance().SaveVariable(key, value.as<bool>());
            }
            GameSaver::GetInstance().SaveToFile();
        });

        luaState.set_function("load_progress", [this](const std::string& key, sol::object defaultValue) -> sol::object {
            GameSaver::GetInstance().LoadFromFile();

            GameSaver::SaveValue value;
            if (defaultValue.is<int>()) {
                value = GameSaver::GetInstance().LoadVariable(key, defaultValue.as<int>());
            } else if (defaultValue.is<float>()) {
                value = GameSaver::GetInstance().LoadVariable(key, defaultValue.as<float>());
            } else if (defaultValue.is<bool>()) {
                value = GameSaver::GetInstance().LoadVariable(key, defaultValue.as<bool>());
            } else {
                return defaultValue;
            }

            if (std::holds_alternative<int>(value)) {
                return sol::make_object(luaState, std::get<int>(value));
            } else if (std::holds_alternative<float>(value)) {
                return sol::make_object(luaState, std::get<float>(value));
            } else if (std::holds_alternative<bool>(value)) {
                return sol::make_object(luaState, std::get<bool>(value));
            }

            return defaultValue;
        });

        # pragma endregion


        RegisterSceneBindings(luaState);
        RegisterPhysicsBindings(luaState);
        RegisterPrefabBindings(luaState);
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
    }

    void LuaBackend::SetWorkingDirectory(const std::filesystem::path& path) {
        
        luaState["package"]["path"] = dafaultPackagePath + ";" + path.string() + "/?.lua";

    }

} // namespace Coffee