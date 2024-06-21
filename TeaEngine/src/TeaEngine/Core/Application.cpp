#include "TeaEngine/Core/Application.h"

namespace Tea
{

    Application::Application()
    {
        m_Window = Window::Create(WindowProps("Tea Engine"));
    }

    Application::~Application()
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