#include "LuaBackend.h"

#include "Bindings/LuaComponents.h"
#include "Bindings/LuaEntity.h"
#include "Bindings/LuaInput.h"
#include "Bindings/LuaLog.h"
#include "Bindings/LuaMath.h"
#include "Bindings/LuaPhysics.h"
#include "Bindings/LuaPrefab.h"
#include "Bindings/LuaScene.h"
#include "Bindings/LuaTimer.h"
#include "CoffeeEngine/Core/Log.h"
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
        BindControllerCodesToLua(luaState, inputTable);
        BindAxisCodesToLua(luaState, inputTable);
        BindInputActionsToLua(luaState, inputTable);

        inputTable.set_function("is_key_pressed", [](KeyCode key) {
            return Input::IsKeyPressed(key);
        });

        inputTable.set_function("is_mouse_button_pressed", [](MouseCode button) {
            return Input::IsMouseButtonPressed(button);
        });

        inputTable.set_function("is_button_pressed", [](ButtonCode button) {
            return Input::GetButtonRaw(button);
        });

        inputTable.set_function("get_axis_position", [](AxisCode axis) {
            return Input::GetAxisRaw(axis);
        });

        inputTable.set_function("get_mouse_position", []() {
            glm::vec2 mousePosition = Input::GetMousePosition();
            return std::make_tuple(mousePosition.x, mousePosition.y);
        });

        inputTable.set_function("get_axis", [](const std::string& action) {
            return Input::GetBinding(action).AsAxis(false);
        });

        inputTable.set_function("get_direction",[](const std::string& action) {
            return Input::GetBinding(action).AsAxis(true);
        });

        inputTable.set_function("get_button", [](const std::string& action) {
            return Input::GetBinding(action).AsButton();
        });

        inputTable.set_function("send_rumble", [](const float leftPwr, const float rightPwr, float duration) {
           Input::SendRumble(leftPwr, rightPwr, duration);
        });

        luaState["Input"] = inputTable;
        # pragma endregion

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
                        COFFEE_CORE_ERROR("Timer callback error: {0}", err.what());
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
                    COFFEE_CORE_ERROR("Timer callback error: {0}", err.what());
                }
            });
        });

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
            "max", [](const glm::vec2& a, const glm::vec2& b) { return (glm::max)(a, b); },
            "min", [](const glm::vec2& a, const glm::vec2& b) { return (glm::min)(a, b); },
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
            "max", [](const glm::vec3& a, const glm::vec3& b) { return (glm::max)(a, b); },
            "min", [](const glm::vec3& a, const glm::vec3& b) { return (glm::min)(a, b); },
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
            "max", [](const glm::vec4& a, const glm::vec4& b) { return (glm::max)(a, b); },
            "min", [](const glm::vec4& a, const glm::vec4& b) { return (glm::min)(a, b); },
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
            "perspective", [](float fovy, float aspect, float nearPlane, float farPlane) { return glm::perspective(fovy, aspect, nearPlane, farPlane); },
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

        #pragma region Bind Entity Functions
        luaState.new_usertype<Entity>("Entity",
            sol::constructors<Entity(), Entity(entt::entity, Scene*)>(),
            "add_component", [this](Entity* self, const std::string& componentName) {
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
                } else if (componentName == "ScriptComponent") {
                    self->AddComponent<ScriptComponent>();
                }else if (componentName == "ParticlesSystemComponent"){
                    self->AddComponent<ParticlesSystemComponent>();
                } else if (componentName == "AudioSourceComponent") {
                    self->AddComponent<AudioSourceComponent>();
                }
                else if (componentName == "RigidbodyComponent")
                {
                    if (!self->HasComponent<RigidbodyComponent>())
                    {
                        try
                        {
                            Ref<BoxCollider> collider = CreateRef<BoxCollider>(glm::vec3(1.0f, 1.0f, 1.0f));

                            RigidBody::Properties props;
                            props.type = RigidBody::Type::Dynamic;
                            props.mass = 1.0f;
                            props.useGravity = true;

                            auto& rbComponent = self->AddComponent<RigidbodyComponent>(props, collider);

                            if (self->HasComponent<TransformComponent>())
                            {
                                auto& transform = self->GetComponent<TransformComponent>();
                                rbComponent.rb->SetPosition(transform.GetLocalPosition());
                                rbComponent.rb->SetRotation(transform.GetLocalRotation());
                            }

                            SceneManager::GetActiveScene()->GetPhysicsWorld().addRigidBody(
                                rbComponent.rb->GetNativeBody());

                            // Set user pointer for collision detection
                            rbComponent.rb->GetNativeBody()->setUserPointer(
                                reinterpret_cast<void*>(static_cast<uintptr_t>((entt::entity)*self)));

                            // Try to automatically size the collider to the mesh AABB
                            ResizeColliderToFitMeshAABB(*self, rbComponent);
                        }
                        catch (const std::exception& e)
                        {
                            COFFEE_CORE_ERROR("Exception creating rigidbody: {0}", e.what());
                            if (self->HasComponent<RigidbodyComponent>())
                            {
                                self->RemoveComponent<RigidbodyComponent>();
                            }
                        }
                    }
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
                } else if (componentName == "ScriptComponent") {
                    return sol::make_object(luaState, std::ref(self->GetComponent<ScriptComponent>()));
                } else if (componentName == "ParticlesSystemComponent") {
                    return sol::make_object(luaState, std::ref(self->GetComponent<ParticlesSystemComponent>()));
                } else if (componentName == "NavigationAgentComponent") {
                    return sol::make_object(luaState, std::ref(self->GetComponent<NavigationAgentComponent>()));
                } else if (componentName == "RigidbodyComponent") {
                    return sol::make_object(luaState, std::ref(self->GetComponent<RigidbodyComponent>()));
                } else if (componentName == "AudioSourceComponent") {
                    return sol::make_object(luaState, std::ref(self->GetComponent<AudioSourceComponent>()));
                } else if (componentName == "AnimatorComponent") {
                    return sol::make_object(luaState, std::ref(self->GetComponent<AnimatorComponent>()));
                } else if (componentName == "UIImageComponent") {
                    return sol::make_object(luaState, std::ref(self->GetComponent<UIImageComponent>()));
                } else if (componentName == "UITextComponent") {
                    return sol::make_object(luaState, std::ref(self->GetComponent<UITextComponent>()));
                } else if (componentName == "UIToggleComponent") {
                    return sol::make_object(luaState, std::ref(self->GetComponent<UIToggleComponent>()));
                } else if (componentName == "UIButtonComponent") {
                    return sol::make_object(luaState, std::ref(self->GetComponent<UIButtonComponent>()));
                } else if (componentName == "UISliderComponent") {
                    return sol::make_object(luaState, std::ref(self->GetComponent<UISliderComponent>()));
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

        luaState.set_function("set_music_volume", [](const sol::object& value) {
            float volume = std::clamp(value.as<float>(), 0.0f, 1.0f);
            Audio::SetBusVolume("Music", volume);
        });

        luaState.set_function("set_sfx_volume", [](const sol::object& value) {
            float volume = std::clamp(value.as<float>(), 0.0f, 1.0f);
            Audio::SetBusVolume("SFX", volume);
        });

        luaState.new_usertype<AudioSourceComponent>("AudioSourceComponent",
        sol::constructors<AudioSourceComponent(), AudioSourceComponent()>(),
         "set_volume", &AudioSourceComponent::SetVolume,
         "play", &AudioSourceComponent::Play,
         "pause", &AudioSourceComponent::Stop);

        luaState.new_usertype<UIImageComponent>("UIImageComponent", sol::constructors<UIImageComponent()>(),
            "set_color", [](UIImageComponent& self, const glm::vec4& color) { self.Color = color; },
            "set_rect", [](UIImageComponent& self, const glm::vec4& uvRect) { self.UVRect = uvRect; }
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

        # pragma region Bind Scene Functions

        luaState.new_usertype<Scene>("Scene",
            "create_entity", &Scene::CreateEntity,
            "destroy_entity", &Scene::DestroyEntity,
            "duplicate_entity", &Scene::Duplicate,
            "get_entity_by_name", &Scene::GetEntityByName,
            "get_all_entities", &Scene::GetAllEntities
        );

        luaState.new_usertype<SceneManager>("SceneManager",
            "preload_scene", [](const std::string& scenePath) {
                return SceneManager::PreloadScene(scenePath);
            },
            "preload_scene_async", [](const std::string& scenePath) {
                return SceneManager::PreloadSceneAsync(scenePath);
            },
            "change_scene", sol::overload(
                [](const std::string& scenePath) {
                    AudioZone::RemoveAllReverbZones();
                    Audio::UnregisterAllGameObjects();
                    SceneManager::ChangeScene(scenePath);
                },
                [](const Ref<Scene>& scene) {
                    AudioZone::RemoveAllReverbZones();
                    Audio::UnregisterAllGameObjects();
                    SceneManager::ChangeScene(scene);
                }
            ),
            "change_scene_async", [](const std::string& scenePath) {
                SceneManager::ChangeSceneAsync(scenePath);
            }
        );
        

        # pragma endregion

        # pragma region Bind Physics Functions

        // Bind RigidBody::Type enum
        luaState.new_enum<RigidBody::Type>("RigidBodyType",
        {
            {"Static", RigidBody::Type::Static},
            {"Dynamic", RigidBody::Type::Dynamic},
            {"Kinematic", RigidBody::Type::Kinematic}
        });

        // Bind RigidBody properties
        luaState.new_usertype<RigidBody::Properties>("RigidBodyProperties",
            sol::constructors<RigidBody::Properties()>(),
            "type", &RigidBody::Properties::type,
            "mass", &RigidBody::Properties::mass,
            "useGravity", &RigidBody::Properties::useGravity,
            "freezeX", &RigidBody::Properties::freezeX,
            "freezeY", &RigidBody::Properties::freezeY,
            "freezeZ", &RigidBody::Properties::freezeZ,
            "freezeRotX", &RigidBody::Properties::freezeRotX,
            "freezeRotY", &RigidBody::Properties::freezeRotY,
            "freezeRotZ", &RigidBody::Properties::freezeRotZ,
            "isTrigger", &RigidBody::Properties::isTrigger,
            "velocity", &RigidBody::Properties::velocity,
            "friction", &RigidBody::Properties::friction,
            "linearDrag", &RigidBody::Properties::linearDrag,
            "angularDrag", &RigidBody::Properties::angularDrag
        );

        // Bind RigidBody methods
        luaState.new_usertype<RigidBody>("RigidBody",
            // Position and rotation
            "set_position", &RigidBody::SetPosition,
            "get_position", &RigidBody::GetPosition,
            "set_rotation", &RigidBody::SetRotation,
            "get_rotation", &RigidBody::GetRotation,

            // Velocity and forces
            "set_velocity", &RigidBody::SetVelocity,
            "get_velocity", &RigidBody::GetVelocity,
            "add_velocity", &RigidBody::AddVelocity,
            "apply_force", &RigidBody::ApplyForce,
            "apply_impulse", &RigidBody::ApplyImpulse,
            "reset_velocity", &RigidBody::ResetVelocity,
            "clear_forces", &RigidBody::ClearForces,

            // Torque and angular velocity methods
            "apply_torque", &RigidBody::ApplyTorque,
            "apply_torque_impulse", &RigidBody::ApplyTorqueImpulse,
            "set_angular_velocity", &RigidBody::SetAngularVelocity,
            "get_angular_velocity", &RigidBody::GetAngularVelocity,

            // Collisions and triggers
            "set_trigger", &RigidBody::SetTrigger,

            // Body type
            "get_body_type", &RigidBody::GetBodyType,
            "set_body_type", &RigidBody::SetBodyType,

            // Mass
            "get_mass", &RigidBody::GetMass,
            "set_mass", &RigidBody::SetMass,

            // Gravity
            "get_use_gravity", &RigidBody::GetUseGravity,
            "set_use_gravity", &RigidBody::SetUseGravity,

            // Constraints
            "get_freeze_x", &RigidBody::GetFreezeX,
            "set_freeze_x", &RigidBody::SetFreezeX,
            "get_freeze_y", &RigidBody::GetFreezeY,
            "set_freeze_y", &RigidBody::SetFreezeY,
            "get_freeze_z", &RigidBody::GetFreezeZ,
            "set_freeze_z", &RigidBody::SetFreezeZ,
            "get_freeze_rot_x", &RigidBody::GetFreezeRotX,
            "set_freeze_rot_x", &RigidBody::SetFreezeRotX,
            "get_freeze_rot_y", &RigidBody::GetFreezeRotY,
            "set_freeze_rot_y", &RigidBody::SetFreezeRotY,
            "get_freeze_rot_z", &RigidBody::GetFreezeRotZ,
            "set_freeze_rot_z", &RigidBody::SetFreezeRotZ,

            // Physical properties
            "get_friction", &RigidBody::GetFriction,
            "set_friction", &RigidBody::SetFriction,
            "get_linear_drag", &RigidBody::GetLinearDrag,
            "set_linear_drag", &RigidBody::SetLinearDrag,
            "get_angular_drag", &RigidBody::GetAngularDrag,
            "set_angular_drag", &RigidBody::SetAngularDrag,

            // Utility
            "get_is_trigger", &RigidBody::GetIsTrigger
        );

        // Add Collider usertype bindings
        luaState.new_usertype<Collider>("Collider");

        luaState.new_usertype<BoxCollider>("BoxCollider",
            sol::constructors<BoxCollider(), BoxCollider(const glm::vec3&)>(),
            sol::base_classes, sol::bases<Collider>()
        );

        luaState.new_usertype<SphereCollider>("SphereCollider",
            sol::constructors<SphereCollider(), SphereCollider(float)>(),
            sol::base_classes, sol::bases<Collider>()
        );

        luaState.new_usertype<CapsuleCollider>("CapsuleCollider",
            sol::constructors<CapsuleCollider(), CapsuleCollider(float, float)>(),
            sol::base_classes, sol::bases<Collider>()
        );

        // Helper functions for creating colliders and rigidbodies
        luaState.set_function("create_box_collider", [](const glm::vec3& size) {
            return CreateRef<BoxCollider>(size);
        });

        luaState.set_function("create_sphere_collider", [](float radius) {
            return CreateRef<SphereCollider>(radius);
        });

        luaState.set_function("create_capsule_collider", [](float radius, float height) {
            return CreateRef<CapsuleCollider>(radius, height);
        });

        luaState.set_function("create_rigidbody", [](const RigidBody::Properties& props, const Ref<Collider>& collider) {
            return RigidBody::Create(props, collider);
        });

        sol::table physicsTable = luaState.create_table();
        luaState["Physics"] = physicsTable;

        // Bind RaycastHit type to Lua
        luaState.new_usertype<RaycastHit>(
            "RaycastHit",
            "hasHit", &RaycastHit::hasHit,
            "hitEntity", &RaycastHit::hitEntity,
            "hitPoint", &RaycastHit::hitPoint,
            "hitNormal", &RaycastHit::hitNormal,
            "hitFraction", &RaycastHit::hitFraction
        );

        // Bind Raycast functions
        physicsTable["Raycast"] = [](const glm::vec3& origin, const glm::vec3& direction, float maxDistance) -> RaycastHit {
            auto scene = SceneManager::GetActiveScene();
            if (!scene)
                return RaycastHit{};

            return scene->GetPhysicsWorld().Raycast(origin, direction, maxDistance);
        };

        physicsTable["RaycastAll"] = [](const glm::vec3& origin, const glm::vec3& direction, float maxDistance) -> std::vector<RaycastHit> {
            auto scene = SceneManager::GetActiveScene();
            if (!scene)
                return std::vector<RaycastHit>{};

            return scene->GetPhysicsWorld().RaycastAll(origin, direction, maxDistance);
        };

        physicsTable["RaycastAny"] = [](const glm::vec3& origin, const glm::vec3& direction, float maxDistance) -> bool {
            auto scene = SceneManager::GetActiveScene();
            if (!scene)
                return false;

            return scene->GetPhysicsWorld().RaycastAny(origin, direction, maxDistance);
        };

        physicsTable["DebugDrawRaycast"] = [](const glm::vec3& origin, const glm::vec3& direction, float maxDistance,
            sol::optional<glm::vec4> rayColor, sol::optional<glm::vec4> hitColor) {
            auto scene = SceneManager::GetActiveScene();
            if (!scene)
            return;

            // Default colors if not provided
            glm::vec4 rColor = rayColor.value_or(glm::vec4(1.0f, 0.0f, 0.0f, 1.0f));
            glm::vec4 hColor = hitColor.value_or(glm::vec4(0.0f, 1.0f, 0.0f, 1.0f));

            scene->GetPhysicsWorld().DebugDrawRaycast(origin, direction, maxDistance, rColor, hColor);
        };

        # pragma endregion
        
        # pragma region Prefab Functions
        // Helper function to load and instantiate a prefab in one call
        luaState.set_function("instantiate_prefab", [](const std::string& path, sol::optional<glm::mat4> transform) -> Entity {
            auto scene = SceneManager::GetActiveScene().get();
            if (!scene) {
                COFFEE_CORE_ERROR("Cannot instantiate prefab: no active scene");
                return Entity();
            }
            
            // Resolve the path (relative to project directory if not absolute)
            std::string resolvedPath = path;
            if (!path.empty() && path[0] != '/') {
                auto projectDir = Project::GetActive()->GetProjectDirectory();
                resolvedPath = (projectDir / path).string();
            }
            
            auto prefab = Prefab::Load(resolvedPath);
            if (!prefab) {
                COFFEE_CORE_ERROR("Failed to load prefab: {0} (resolved to {1})", path, resolvedPath);
                return Entity();
            }
            
            return prefab->Instantiate(scene, transform.value_or(glm::mat4(1.0f)));
        });

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
    }

    void LuaBackend::SetWorkingDirectory(const std::filesystem::path& path) {
        
        luaState["package"]["path"] = dafaultPackagePath + ";" + path.string() + "/?.lua";

    }

} // namespace Coffee