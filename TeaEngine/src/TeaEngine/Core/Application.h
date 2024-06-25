#pragma once

#include "Base.h"
#include "Window.h"
#include "LayerStack.h"
#include <TeaEngine/Events/ApplicationEvent.h>
namespace Tea
{

    class Application
    {
      public:
        Application();
        virtual ~Application();

        void Run();

        void OnEvent(Event& e);
        void PushLayer(Layer* layer);
        void PushOverlay(Layer* layer);
        private:
        bool OnWindowClose(WindowCloseEvent& e);

        Scope<Window> m_Window;
        bool m_Running = true;
        LayerStack m_LayerStack;
    };

    // To be defined in CLIENT
    Application* CreateApplication();

} // namespace Tea
