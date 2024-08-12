#pragma once

#include "Base.h"
#include "Window.h"
#include "LayerStack.h"
#include "TeaEngine/Events/ApplicationEvent.h"

#include "TeaEngine/ImGui/ImGuiLayer.h"

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

        Window& GetWindow() { return *m_Window; }

        void Close();

        ImGuiLayer* GetImGuiLayer() { return m_ImGuiLayer; }

        static Application& Get() { return *s_Instance; }

        private:
        bool OnWindowClose(WindowCloseEvent& e);

        Scope<Window> m_Window;
        ImGuiLayer* m_ImGuiLayer;
        bool m_Running = true;
        LayerStack m_LayerStack;
        float m_LastFrameTime = 0.0f;

      private:
        static Application* s_Instance;
    };

    // To be defined in CLIENT
    Application* CreateApplication();

} // namespace Tea
