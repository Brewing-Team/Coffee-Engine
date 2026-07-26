#include "CoffeeEngine/Core/Engine.h"

#include "CoffeeEngine/Core/Application.h"
#include "CoffeeEngine/Core/Base.h"
#include "CoffeeEngine/Core/Layer.h"
#include "CoffeeEngine/Core/Stopwatch.h"
#include "CoffeeEngine/Core/Input.h"
#include "CoffeeEngine/Events/ControllerEvent.h"
#include "CoffeeEngine/Events/KeyEvent.h"
#include "CoffeeEngine/Events/MouseEvent.h"
#include "CoffeeEngine/ImGui/ImGuiLayer.h"
#include "CoffeeEngine/Rendering/Renderer.h"
#include "CoffeeEngine/Audio/Audio.h"

#include <SDL3/SDL.h>
#include <SDL3/SDL_timer.h>
#include <tracy/Tracy.hpp>

#ifdef WIN32
#include <windows.h>
extern "C"
{
    __declspec(dllexport) DWORD NvOptimusEnablement = 0x00000001;
    __declspec(dllexport) int AmdPowerXpressRequestHighPerformance = 1;
}
#endif //def WIN32

namespace Coffee
{
    Engine::Engine()
        : m_Window(Window::Create(WindowProps("Coffee Engine")))
        , m_RendererAPI()
        , m_Renderer(&m_RendererAPI)
        , m_Input(m_Window.get())
        , m_Audio()
        , m_ResourceManager()
        , m_SceneManager()
        , m_ProjectManager(&m_SceneManager, &m_ResourceManager, &m_Scripting, &m_Audio) // Info: I know with this design project manager should go after scripting but is temporal until we have a event system.
        , m_Scripting()

    {
        ZoneScoped;

        SetEventCallback(COFFEE_BIND_EVENT_FN(OnEvent));

        auto imguiLayer = CreateScope<ImGuiLayer>();
        m_ImGuiLayer = imguiLayer.get();

		PushOverlay(std::move(imguiLayer));
    }

    void Engine::PushLayer(Scope<Layer> layer)
    {
        ZoneScoped;

        m_LayerStack.PushLayer(std::move(layer));
        layer->OnAttach();
    }

    void Engine::PushOverlay(Scope<Layer> layer)
    {
        ZoneScoped;
        
        m_LayerStack.PushOverlay(std::move(layer));
        layer->OnAttach();
    }

    void Engine::Close()
    {
        m_Running = false;
    }

    void Engine::OnEvent(Event& e)
    {
        ZoneScoped;

        EventDispatcher dispacher(e);
        dispacher.Dispatch<WindowCloseEvent>(COFFEE_BIND_EVENT_FN(OnWindowClose));

        for (auto it = m_LayerStack.end(); it != m_LayerStack.begin();)
        {
            (*--it)->OnEvent(e);
            if(e.Handled)
                break;
        }

        m_Input.OnEvent(e);
    }

    void Engine::Run(Application& app)
    {
        ZoneScoped;

        static Stopwatch frameTimeStopwatch;

        app.OnInit(*this);

        while (m_Running)
        {   
            ZoneScopedN("RunLoop");

            m_LastFrameTime = frameTimeStopwatch.GetPreciseElapsedTime();
            frameTimeStopwatch.Reset();
            frameTimeStopwatch.Start();

            float deltaTime = m_LastFrameTime;

            //Poll and handle events
            ProcessEvents();

            //Process audio
            m_Audio.ProcessAudio();

            //Prepare input frame
            m_Input.OnFrameUpdate();

            app.OnUpdate(deltaTime);

            //Update and render
            {
                ZoneScopedN("LayerStack Update");

                for(Scope<Layer>& layer : m_LayerStack)
                    layer->OnUpdate(deltaTime);
            }

            m_Renderer.Render();

            //Render ImGui
            m_ImGuiLayer->Begin();
            {
                ZoneScopedN("LayerStack ImGuiRender");

                for(Scope<Layer>& layer : m_LayerStack)
                    layer->OnImGuiRender();
            }
            m_ImGuiLayer->End();

            m_Window->OnUpdate();
        }
    }

    void Engine::ProcessEvents()
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
                case SDL_EVENT_DROP_FILE:
                {
                    const std::string& source = event.drop.source ? event.drop.source : "";
                    const std::string& file = event.drop.data ? event.drop.data : "";

                    FileDropEvent e(event.drop.timestamp, event.drop.windowID,event.drop.x,
                                    event.drop.y, source, file);
                    m_EventCallback(e);
                    break;
                }
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
                case SDL_EVENT_GAMEPAD_ADDED:
                {
                    ControllerAddEvent e((event.gdevice.which));
                    m_EventCallback(e);
                    break;
                }
                case SDL_EVENT_GAMEPAD_REMOVED:
                {
                    ControllerRemoveEvent e(event.gdevice.which);
                    m_EventCallback(e);
                    break;
                }
                case SDL_EVENT_GAMEPAD_BUTTON_DOWN:
                {
                    ButtonPressEvent e(event.gbutton.which, event.gbutton.button);
                    m_EventCallback(e);
                    break;
                }
                case SDL_EVENT_GAMEPAD_BUTTON_UP:
                {
                    ButtonReleaseEvent e(event.gbutton.which, event.gbutton.button);
                    m_EventCallback(e);
                    break;
                }
                case SDL_EVENT_GAMEPAD_AXIS_MOTION:
                {
                    AxisMoveEvent e(event.gaxis.which, event.gaxis.axis, event.gaxis.value);
                    m_EventCallback(e);
                    break;
                }
            }
        }
    }

    bool Engine::OnWindowClose(WindowCloseEvent& e)
    {
        ZoneScoped;

        m_Running = false;
        return true;
    }

} // namespace Coffee
