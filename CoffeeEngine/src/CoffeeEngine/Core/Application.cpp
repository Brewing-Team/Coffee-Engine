#include "CoffeeEngine/Core/Application.h"
#include "CoffeeEngine/Core/Layer.h"
#include "CoffeeEngine/Core/Log.h"

#include "CoffeeEngine/Core/Input.h"
#include "CoffeeEngine/Renderer/Renderer.h"
#include "CoffeeEngine/Renderer/RendererAPI.h"
#include "SDL3/SDL_timer.h"

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
        m_Window->SetEventCallback(COFFEE_BIND_EVENT_FN(OnEvent));

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

            {
                ZoneScopedN("LayerStack Update");

                for(Layer* layer : m_LayerStack)
                    layer->OnUpdate(deltaTime);
            }

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

    bool Application::OnWindowClose(WindowCloseEvent& e)
    {
        ZoneScoped;

        m_Running = false;
        return true;
    }

} // namespace Coffee
