#include "TeaEngine/Core/Application.h"
#include "TeaEngine/Core/Layer.h"
#include "TeaEngine/Core/Log.h"

#include "TeaEngine/Core/Input.h"
#include "TeaEngine/Renderer/Renderer.h"
#include "TeaEngine/Renderer/RendererAPI.h"

#include <GLFW/glfw3.h>
#include <tracy/Tracy.hpp>

namespace Tea
{
    Application* Application::s_Instance = nullptr;

    Application::Application()
    {
        ZoneScoped;

        TEA_CORE_ASSERT(!s_Instance, "Application already exists!");
		s_Instance = this;

        m_Window = Window::Create(WindowProps("Tea Engine"));
        m_Window->SetEventCallback(TEA_BIND_EVENT_FN(OnEvent));

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
        dispacher.Dispatch<WindowCloseEvent>(TEA_BIND_EVENT_FN(OnWindowClose));

        //TEA_CORE_TRACE("{0}", e);

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

        while (m_Running)
        {   
            ZoneScopedN("RunLoop");

            float time = (float)glfwGetTime();
            float deltaTime = time - m_LastFrameTime;
            m_LastFrameTime = time;

            TEA_CORE_TRACE("Delta Time: {0}", 1 / deltaTime);

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

} // namespace Tea
