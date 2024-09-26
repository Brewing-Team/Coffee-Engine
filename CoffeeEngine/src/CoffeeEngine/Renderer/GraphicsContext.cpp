#include "CoffeeEngine/Renderer/GraphicsContext.h"
#include "SDL3/SDL.h"
#include "SDL3/SDL_video.h"

#include <glad/glad.h>
#include <tracy/Tracy.hpp>

namespace Coffee {

    GraphicsContext::GraphicsContext(SDL_Window* windowHandle) : m_WindowHandle(windowHandle)
    {
		COFFEE_CORE_ASSERT(windowHandle, "Window handle is null!")
    }

    GraphicsContext::~GraphicsContext()
    {
    }

    void GraphicsContext::Init()
    {
        ZoneScoped;

        SDL_GLContext context = SDL_GL_CreateContext(m_WindowHandle);
        SDL_GL_MakeCurrent(m_WindowHandle, context);
        int status = gladLoadGLLoader((GLADloadproc)SDL_GL_GetProcAddress);
		COFFEE_CORE_ASSERT(status, "Failed to initialize Glad!");

        COFFEE_CORE_INFO("OpenGL Info:");
		COFFEE_CORE_INFO("  Vendor: {0}", reinterpret_cast<const char*>(glGetString(GL_VENDOR)));
		COFFEE_CORE_INFO("  Renderer: {0}", reinterpret_cast<const char*>(glGetString(GL_RENDERER)));
		COFFEE_CORE_INFO("  Version: {0}", reinterpret_cast<const char*>(glGetString(GL_VERSION)));

		COFFEE_CORE_ASSERT(GLVersion.major > 4 || (GLVersion.major == 4 && GLVersion.minor >= 5), "Coffee Engine requires at least OpenGL version 4.5!");
	}

    void GraphicsContext::SwapBuffers()
    {
        ZoneScoped;

        SDL_GL_SwapWindow(m_WindowHandle);

        FrameMark;
    }

    Scope<GraphicsContext> GraphicsContext::Create(SDL_Window* window)
    {
        return CreateScope<GraphicsContext>(window);
    }
}
