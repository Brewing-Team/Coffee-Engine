#include "TeaEngine/Core/Window.h"
#include "TeaEngine/Core/Log.h"

#include <cstddef>
#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>
#include <GL/gl.h>

namespace Tea {

	Window::Window(const WindowProps& props)
	{
		Init(props);
	}

	Window::~Window()
	{
		Shutdown();
	}

	void Window::Init(const WindowProps& props)
	{
		m_Data.Title = props.Title;
		m_Data.Width = props.Width;
		m_Data.Height = props.Height;

		TEA_CORE_INFO("Creating window {0} ({1}, {2})", props.Title, props.Width, props.Height);

        SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS);

        m_Window = SDL_CreateWindow(m_Data.Title.c_str(), 0, 0, m_Data.Width, m_Data.Height, SDL_WINDOW_OPENGL);

        SDL_GLContext context = SDL_GL_CreateContext(m_Window); //Put the variable in my Rendering API
    }

	void Window::Shutdown()
	{
		/* glfwDestroyWindow(m_Window);
		--s_GLFWWindowCount;

		if (s_GLFWWindowCount == 0)
		{
			glfwTerminate();
		} */
	}

	void Window::OnUpdate()
	{
		SDL_PumpEvents();
	}

	void Window::SetVSync(bool enabled)
	{
		if (enabled)
			SDL_GL_SetSwapInterval(1);
		else
			SDL_GL_SetSwapInterval(0);

		m_Data.VSync = enabled;
	}

	bool Window::IsVSync() const
	{
		return m_Data.VSync;
	}

}