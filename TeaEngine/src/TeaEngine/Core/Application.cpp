#include "TeaEngine/Core/Application.h"

namespace Tea
{

    Application::Application()
    {
        m_Window = Window::Create(WindowProps("Hello Tea Engine!"));
    }

    Application::~Application()
    {
    }

    void Application::Run()
    {
        while (true)
            ;
    }

} // namespace Tea