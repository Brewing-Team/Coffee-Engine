#include "CoffeeEngine/Core/Application.h"
#include "CoffeeEngine/Core/Layer.h"
#include "CoffeeEngine/Events/KeyEvent.h"
#include "CoffeeEngine/Renderer/Renderer.h"

#include <SDL3/SDL_timer.h>
#include <SDL3/SDL.h>
#include <tracy/Tracy.hpp>

namespace Coffee
{
    Application* Application::s_Instance = nullptr;

    Application::Application()
    {
        ZoneScoped;

        COFFEE_CORE_ASSERT(!s_Instance, "Application already exists!");
		s_Instance = this;

        m_Window = Window::Create(WindowProps("Coffee Engine"));
        SetEventCallback(COFFEE_BIND_EVENT_FN(OnEvent));

        //TODO: Create The Renderer Instance and remove the RendererAPI Instance
        Renderer::Init();

        m_ImGuiLayer = new ImGuiLayer();
		PushOverlay(m_ImGuiLayer);
    }

    Application::~Application()
    {
    }

    void Application::PushLayer(Layer* layer)
    {
        ZoneScoped;

        m_LayerStack.PushLayer(layer);
        layer->OnAttach();
    }

    void Application::PushOverlay(Layer* layer)
    {
        ZoneScoped;
        
        m_LayerStack.PushOverlay(layer);
        layer->OnAttach();
    }

    void Application::Close()
    {
        m_Running = false;
    }

    void Application::OnEvent(Event& e)
    {
        ZoneScoped;

        EventDispatcher dispacher(e);
        dispacher.Dispatch<WindowCloseEvent>(COFFEE_BIND_EVENT_FN(OnWindowClose));

        //COFFEE_CORE_TRACE("{0}", e);

        for (auto it = m_LayerStack.end(); it != m_LayerStack.begin();)
        {
            (*--it)->OnEvent(e);
            if(e.Handled)
                break;
        }
    }

    void Application::Run()
    {
        ZoneScoped;

        Uint64 frequency = SDL_GetPerformanceFrequency();

        while (m_Running)
        {   
            ZoneScopedN("RunLoop");

            //TODO: Improve precision using double instead of float
            float time = (float)SDL_GetPerformanceCounter();
            float deltaTime = (time - m_LastFrameTime) / frequency;
            m_LastFrameTime = time;         

            //Poll and handle events
            ProcessEvents();

            //Update and render
            {
                ZoneScopedN("LayerStack Update");

                for(Layer* layer : m_LayerStack)
                    layer->OnUpdate(deltaTime);
            }

            //Render ImGui
            m_ImGuiLayer->Begin();
            {
                ZoneScopedN("LayerStack ImGuiRender");

                for(Layer* layer : m_LayerStack)
                    layer->OnImGuiRender();
            }
            m_ImGuiLayer->End();

            m_Window->OnUpdate();
        }
    }

    void Application::ProcessEvents()
    {
        SDL_Event event;
        while(SDL_PollEvent(&event))
        {
            m_ImGuiLayer->ProcessEvents(event);
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

    bool Application::OnWindowClose(WindowCloseEvent& e)
    {
        ZoneScoped;

        m_Running = false;
        return true;
    }

} // namespace Coffee
