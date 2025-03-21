#include "CoffeeEngine/Core/Input.h"

#include "CoffeeEngine/Events/ControllerEvent.h"
#include "CoffeeEngine/Events/Event.h"
#include "CoffeeEngine/Events/KeyEvent.h"
#include "CoffeeEngine/Events/MouseEvent.h"
#include "CoffeeEngine/Project/Project.h"
#include "SDL3/SDL_keyboard.h"
#include "SDL3/SDL_mouse.h"
#include "imgui_internal.h"

#include <SDL3/SDL_init.h>

#include <cereal/types/vector.hpp>
#include <cereal/types/unordered_map.hpp>
#include <cereal/archives/json.hpp>
#include <fstream>

namespace Coffee {

    constexpr const char* MAPPING_FILE_PATH = "InputMapping.json";

    std::unordered_map<std::string, InputBinding> Input::m_BindingsMap = std::unordered_map<std::string, InputBinding>();
    std::vector<Ref<Gamepad>> Input::m_Gamepads;
    std::unordered_map<ButtonCode, uint8_t> Input::m_ButtonStates = {{Button::Invalid, 0}};
    std::unordered_map<AxisCode, float> Input::m_AxisStates = {{Axis::Invalid, 0.0f}};
    std::unordered_map<AxisCode, float> Input::m_AxisDeadzones;
    std::unordered_map<KeyCode, bool> Input::m_KeyStates = {{Key::Unknown, false}};
    std::unordered_map<MouseCode, bool> Input::m_MouseStates;
    glm::vec2 Input::m_MousePosition = glm::vec2(0.0f);

    void Input::Init()
    {
        SDL_InitSubSystem(SDL_INIT_GAMEPAD);

        // Axis deadzone defaults
        m_AxisDeadzones[Axis::LeftTrigger] = 0.15f;
        m_AxisDeadzones[Axis::RightTrigger] = 0.15f;
        m_AxisDeadzones[Axis::LeftX] = 0.15f;
        m_AxisDeadzones[Axis::RightX] = 0.15f;
        m_AxisDeadzones[Axis::LeftY] = 0.15f;
        m_AxisDeadzones[Axis::RightY] = 0.15f;
    }

    void Input::Save()
    {
        // Can't save project input mapping if there's no project
        if (Project::GetActive() == nullptr)
            return;

        auto path = Project::GetProjectDirectory() / MAPPING_FILE_PATH;

        std::ofstream file(path);

        cereal::JSONOutputArchive archive(file);

        archive(m_BindingsMap);
    }

    void Input::Load()
    {
        if (Project::GetActive() == nullptr)
            return;

        auto path = Project::GetProjectDirectory() / MAPPING_FILE_PATH;

        if (!std::filesystem::exists(path))
        {
            COFFEE_INFO("Mappings file not found, generating...");
            Input::GenerateDefaultMappingFile();
            return;
        }

        std::ifstream file(path);

        cereal::JSONInputArchive archive(file);

        archive(m_BindingsMap);

        COFFEE_INFO("Loaded input mappings");
    }

    bool Input::IsKeyPressed(const KeyCode key)
    {
        return m_KeyStates[key];
        //const bool* state = SDL_GetKeyboardState(nullptr);
        //return state[key];
    }

    bool Input::IsMouseButtonPressed(const MouseCode button)
    {
        return m_MouseStates[button];
        // return SDL_GetMouseState(nullptr, nullptr) & SDL_BUTTON_MASK(button);
    }

    const glm::vec2& Input::GetMousePosition()
    {
        return m_MousePosition;
        // float x, y;
        // SDL_GetMouseState(&x, &y);
        // return {x, y};
    }

    const float Input::GetMouseX()
    {
        return GetMousePosition().x;
    }

    const float Input::GetMouseY()
    {
        return GetMousePosition().y;
    }

    bool Input::GetButtonRaw(const ButtonCode button)
    {
        return m_ButtonStates[button];
    }

    float Input::GetAxisRaw(const AxisCode axis)
    {
        return m_AxisStates[axis];
    }
    InputBinding& Input::GetBinding(const std::string& actionName)
    {
        return m_BindingsMap[actionName];
    }

    void Input::OnAddController(const ControllerAddEvent* cEvent)
    {
        m_Gamepads.emplace_back(new Gamepad(cEvent->Controller));
    }


    void Input::OnRemoveController(const ControllerRemoveEvent* cEvent)
    {
        // Remove controller by SDL_Gamepad ID
        auto pred = [&cEvent](const Ref<Gamepad>& gamepad) {
            return gamepad->getId() == cEvent->Controller;
        };
        erase_if(m_Gamepads, pred);
    }
    void Input::OnButtonPressed(const ButtonPressEvent& e) {
        m_ButtonStates[e.Button] += 1;
    }

    void Input::OnButtonReleased(const ButtonReleaseEvent& e) {
        m_ButtonStates[e.Button] -= 1;
    }

    void Input::OnAxisMoved(const AxisMoveEvent& e) {

        float deadzone = m_AxisDeadzones[e.Axis];
        float normalizedValue = e.Value / 32767.0f;

        // TODO fix axis curve (currently starts at whatever the deadzone value is instead of at slightly larger than 0)
        if (std::abs(normalizedValue) < deadzone)
        {
            normalizedValue = 0.0f;
        }
       
        m_AxisStates[e.Axis] = normalizedValue;
    }
    void Input::OnKeyPressed(const KeyPressedEvent& kEvent) {
        m_KeyStates[kEvent.GetKeyCode()] = true;
    }

    void Input::OnKeyReleased(const KeyReleasedEvent& kEvent) {
        m_KeyStates[kEvent.GetKeyCode()] = false;
    }

    void Input::OnMouseButtonPressed(const MouseButtonPressedEvent& mEvent) {
        m_MouseStates[mEvent.GetMouseButton()] = true;
    }

    void Input::OnMouseButtonReleased(const MouseButtonReleasedEvent& mEvent) {
        m_MouseStates[mEvent.GetMouseButton()] = false;
    }

    void Input::OnMouseMoved(const MouseMovedEvent& mEvent) {
        m_MousePosition.x = mEvent.GetX();
        m_MousePosition.y = mEvent.GetY();
    }

    void Input::OnEvent(Event& e)
    {   
        if(e.Handled)
            return;

        // TODO change this code for an event dispatcher
        if (e.IsInCategory(EventCategoryInput))
        {
            switch (e.GetEventType())
            {
                using enum EventType;
            case ControllerConnected: {
                if (const auto* cEvent = static_cast<ControllerAddEvent*>(&e))
                    OnAddController(cEvent);
                break;
            }
            case ControllerDisconnected: {
                if (const auto* cEvent = static_cast<ControllerRemoveEvent*>(&e))
                    OnRemoveController(cEvent);
                break;
            }
            case ButtonPressed: {
                if (const auto* bEvent = static_cast<ButtonPressEvent*>(&e))
                    OnButtonPressed(*bEvent);
                break;
            }
            case ButtonReleased: {
                if (const auto* bEvent = static_cast<ButtonReleaseEvent*>(&e))
                    OnButtonReleased(*bEvent);
                break;
            }
            case AxisMoved: {
                if (const auto* aEvent = static_cast<AxisMoveEvent*>(&e))
                    OnAxisMoved(*aEvent);
                break;
            }
            case KeyPressed: {
                if (const auto* kEvent = static_cast<KeyPressedEvent*>(&e))
                    OnKeyPressed(*kEvent);
                break;
            }
            case KeyReleased: {
                if (const auto* kEvent = static_cast<KeyReleasedEvent*>(&e))
                    OnKeyReleased(*kEvent);
                break;
            }
            case MouseButtonPressed: {
                if (const auto* mEvent = static_cast<MouseButtonPressedEvent*>(&e))
                    OnMouseButtonPressed(*mEvent);
                break;
            }
            case MouseButtonReleased: {
                if (const auto* mEvent = static_cast<MouseButtonReleasedEvent*>(&e))
                    OnMouseButtonReleased(*mEvent);
                break;
            }
            case MouseMoved: {
                if (const auto* mEvent = static_cast<MouseMovedEvent*>(&e))
                    OnMouseMoved(*mEvent);
                break;
            }

            default: {
                break;
            }
            }
        }
    }

    void Input::GenerateDefaultMappingFile()
    {
        #pragma region Defaults

        //UI defaults
        m_BindingsMap["UiX"]//.SetName("UiX")
            .SetAxis(Axis::LeftX)
            .SetButtonNeg(Button::DpadLeft).SetButtonPos(Button::DpadRight)
            .SetPosKey(Key::D).SetNegKey(Key::A);

        m_BindingsMap["UiY"]//.SetName("UiY")
            .SetAxis(Axis::LeftY)
            .SetButtonNeg(Button::DpadDown).SetButtonPos(Button::DpadUp)
            .SetPosKey(Key::W).SetNegKey(Key::S);

        m_BindingsMap["Cancel"]//.SetName("Cancel")
            .SetButtonPos(Button::East)
            .SetPosKey(Key::RShift);

        m_BindingsMap["Confirm"]//.SetName("Confirm")
            .SetButtonPos(Button::South)
            .SetPosKey(Key::Return);


        // Gameplay defaults
        m_BindingsMap["MoveX"]//.SetName("MoveX")
            .SetAxis(Axis::LeftX)
            .SetNegKey(Key::A).SetPosKey(Key::D);

        m_BindingsMap["MoveY"]//.SetName("MoveY")
            .SetAxis(Axis::LeftY)
            .SetNegKey(Key::S).SetPosKey(Key::W);

        m_BindingsMap["AimX"]//.SetName("AimX")
            .SetAxis(Axis::RightX)
            .SetNegKey(Key::Kp4).SetPosKey(Key::Kp6);

        m_BindingsMap["AimY"]//.SetName("AimY")
            .SetAxis(Axis::RightY)
            .SetNegKey(Key::Kp2).SetPosKey(Key::Kp8);

        m_BindingsMap["Shoot"]//.SetName("Shoot")
            .SetAxis(Axis::RightTrigger)
            .SetPosKey(Key::Kp0);

        m_BindingsMap["Melee"]//.SetName("Melee")
            .SetButtonPos(Button::RightShoulder)
            .SetPosKey(Key::E);

        m_BindingsMap["Interact"]//.SetName("Interact")
            .SetButtonPos(Button::South)
            .SetPosKey(Key::V);

        m_BindingsMap["Dash"]//.SetName("Dash")
            .SetButtonPos(Button::East)
            .SetPosKey(Key::Space);

        m_BindingsMap["Cover"]//.SetName("Cover")
            .SetButtonPos(Button::West)
            .SetPosKey(Key::C);

        m_BindingsMap["Skill1"]//.SetName("Skill1")
            .SetButtonPos(Button::North)
            .SetPosKey(Key::D1);

        m_BindingsMap["Skill2"]//.SetName("Skill2")
            .SetButtonPos(Button::LeftShoulder)
            .SetPosKey(Key::D2);

        m_BindingsMap["Skill3"]//.SetName("Skill3")
            .SetAxis(Axis::LeftTrigger)
            .SetPosKey(Key::D3);

        m_BindingsMap["Injector"]//.SetName("Injector")
            .SetButtonPos(Button::DpadUp)
            .SetPosKey(Key::D4);

        m_BindingsMap["Grenade"]//.SetName("Grenade")
            .SetButtonPos(Button::DpadRight)
            .SetPosKey(Key::Q);

        m_BindingsMap["Map"]//.SetName("Map")
            .SetButtonPos(Button::Back)
            .SetPosKey(Key::Tab);

        m_BindingsMap["Pause"]//.SetName("Pause")
            .SetButtonPos(Button::Start)
            .SetPosKey(Key::Escape);


        #pragma endregion

        Input::Save();
    }

} // namespace Coffee