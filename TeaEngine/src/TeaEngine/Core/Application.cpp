#include "TeaEngine/Core/Application.h"
#include "TeaEngine/Core/Layer.h"
#include "TeaEngine/Core/Log.h"

#include "TeaEngine/Core/Input.h"
#include "TeaEngine/Renderer/RendererAPI.h"

namespace Tea
{
    Application* Application::s_Instance = nullptr;

    Application::Application()
    {
        TEA_CORE_ASSERT(!s_Instance, "Application already exists!");
		s_Instance = this;

        m_Window = Window::Create(WindowProps("Tea Engine"));
        m_Window->SetEventCallback(TEA_BIND_EVENT_FN(OnEvent));

        //TODO: Create The Renderer Instance and remove the RendererAPI Instance
        RendererAPI::Init();

        m_ImGuiLayer = new ImGuiLayer();
		PushOverlay(m_ImGuiLayer);
    }

    Application::~Application()
    {
    }

    void Application::PushLayer(Layer* layer)
    {
        m_LayerStack.PushLayer(layer);
        layer->OnAttach();
    }

    void Application::PushOverlay(Layer* layer)
    {
        m_LayerStack.PushOverlay(layer);
        layer->OnAttach();
    }

    void Application::Close()
    {
        m_Running = false;
    }

    void Application::OnEvent(Event& e)
    {
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
        while (m_Running)
        {
            for(Layer* layer : m_LayerStack)
                layer->OnUpdate();
            
            m_ImGuiLayer->Begin();
            for(Layer* layer : m_LayerStack)
                layer->OnImGuiRender();
            m_ImGuiLayer->End();

            m_Window->OnUpdate();
        }
    }

    bool Application::OnWindowClose(WindowCloseEvent& e)
    {
        m_Running = false;
        return true;
    }

} // namespace Tea
