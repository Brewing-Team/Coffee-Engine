#include "TeaEngine/Core/Application.h"

#include <GLFW/glfw3.h>

namespace Tea
{
    #define BIND_EVENT_FN(x) std::bind(&Application::x, this, std::placeholders::_1)

    Application::Application()
    {
        m_Window = Window::Create(WindowProps("Tea Engine"));
        m_Window->SetEventCallback(BIND_EVENT_FN(OnEvent));
    }

    Application::~Application()
    {
    }

    void Application::OnEvent(Event& e)
    {
        EventDispatcher dispacher(e);
        dispacher.Dispatch<WindowCloseEvent>(BIND_EVENT_FN(OnWindowClose));

        TEA_CORE_INFO("{0}", e);
    }

    void Application::Run()
    {
        while (m_Running)
        {
            glClearColor(1,0,1,1);
            glClear(GL_COLOR_BUFFER_BIT);
            m_Window->OnUpdate();
        }
    }

    bool Application::OnWindowClose(WindowCloseEvent& e)
    {
        m_Running = false;
        return true;
    }

} // namespace Tea
