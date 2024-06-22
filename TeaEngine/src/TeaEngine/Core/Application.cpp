#include "TeaEngine/Core/Application.h"

#include <GLFW/glfw3.h>

namespace Tea
{

    Application::Application()
    {
        m_Window = Window::Create(WindowProps("Tea Engine"));
        m_Window->SetEventCallback(std::bind)
    }

    Application::~Application()
    {
    }

    void Application::OnEvent(Event& e)
    {

    }

    void Application::Run()
    {
        while (m_Running)
        {
            m_Window->OnUpdate();
        }
    }

} // namespace Tea