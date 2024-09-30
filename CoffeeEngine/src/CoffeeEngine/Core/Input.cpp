#include "CoffeeEngine/Core/Input.h"
#include "CoffeeEngine/Events/ApplicationEvent.h"
#include "CoffeeEngine/Events/KeyEvent.h"
#include "CoffeeEngine/Events/MouseEvent.h"
#include <SDL3/SDL_events.h>
#include <SDL3/SDL_keyboard.h>
#include <SDL3/SDL_mouse.h>
#include <imgui_impl_sdl3.h>

namespace Coffee {

	Input::EventCallbackFn Input::m_EventCallback = nullptr;

	bool Input::IsKeyPressed(const KeyCode key)
	{
		const bool* state = SDL_GetKeyboardState(nullptr);

		return state[key];
	}

	bool Input::IsMouseButtonPressed(const MouseCode button)
	{
		return SDL_GetMouseState(nullptr, nullptr) & SDL_BUTTON(button);
	}

	glm::vec2 Input::GetMousePosition()
	{
		float x, y;
		SDL_GetMouseState(&x, &y);

		return { x, y };
	}

	float Input::GetMouseX()
	{
		return GetMousePosition().x;
	}

	float Input::GetMouseY()
	{
		return GetMousePosition().y;
	}

	void Input::ProcessEvents()
    {
        SDL_Event event;
        while(SDL_PollEvent(&event))
        {
            ImGui_ImplSDL3_ProcessEvent(&event);
            switch (event.type)
            {
                case SDL_EVENT_QUIT:
                {
                    WindowCloseEvent e;
                    m_EventCallback(e);
                    break;
                }
                case SDL_EVENT_WINDOW_RESIZED:
                {
                    WindowResizeEvent e(event.window.data1, event.window.data2);
                    m_EventCallback(e);
                    break;
                }
                case SDL_EVENT_WINDOW_CLOSE_REQUESTED:
                {
                    WindowCloseEvent e;
                    m_EventCallback(e);
                    break;
                }
                    break;
                case SDL_EVENT_KEY_DOWN:
                {
                    if(event.key.repeat)
                    {
                        KeyPressedEvent e(event.key.scancode, 1);
                        m_EventCallback(e);
                    }
                    else
                    {
                        KeyPressedEvent e(event.key.scancode, 0);
                        m_EventCallback(e);
                    }
                    break;
                }
                case SDL_EVENT_KEY_UP:
                {
                    KeyReleasedEvent e(event.key.scancode);
                    m_EventCallback(e);
                    break;
                }
                case SDL_EVENT_MOUSE_BUTTON_DOWN:
                {
                    MouseButtonPressedEvent e(event.button.button);
                    m_EventCallback(e);
                    break;
                }
                case SDL_EVENT_MOUSE_BUTTON_UP:
                {
                    MouseButtonReleasedEvent e(event.button.button);
                    m_EventCallback(e);
                    break;
                }
                case SDL_EVENT_MOUSE_MOTION:
                {
                    MouseMovedEvent e(event.motion.x, event.motion.y);
                    m_EventCallback(e);
                    break;
                }
                case SDL_EVENT_MOUSE_WHEEL:
                {
                    MouseScrolledEvent e(event.wheel.x, event.wheel.y);
                    m_EventCallback(e);
                    break;
                }
            }
        }
    }

}