#include "ImGuiLayer.h"

#include "CoffeeEngine/Core/Application.h"
#include "CoffeeEngine/Core/Window.h"
#include "SDL3/SDL_video.h"

#include <imgui.h>
#include <imgui_internal.h>

//Previously this was in a specific file called ImGuiBuild.cpp but on Windows Platform using MSVC was not linking
#define IMGUI_IMPL_OPENGL_LOADER_GLAD
#include "imgui_impl_sdl3.cpp"
#include "imgui_impl_opengl3.cpp"

#include <tracy/Tracy.hpp>

namespace Coffee {

    ImGuiLayer::ImGuiLayer()
        : Layer("ImGuiLayer")
    {
    }

    ImGuiLayer::~ImGuiLayer()
    {
    }

    void ImGuiLayer::OnAttach()
    {
        ZoneScoped;

		IMGUI_CHECKVERSION();
        ImGui::CreateContext();
        ImGui::StyleColorsDark();

        ImGuiIO& io = ImGui::GetIO(); (void)io;
        io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
        io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
        //io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable; //Comment this for disable the detached imgui windows from the main window

        float fontSize = 18.0f;
        io.FontDefault = io.Fonts->AddFontFromFileTTF("assets/fonts/OpenSans-SemiBold.ttf", fontSize);

		/* float fontSize = 16.0f;
        io.FontDefault = io.Fonts->AddFontFromFileTTF("/home/hugo/.fonts/iosevka-nerd-font.ttf", fontSize); */

		SetTeaColorStyle();

        Application& app = Application::Get();
        SDL_Window* window = static_cast<SDL_Window*>(app.GetWindow().GetNativeWindow());

        ImGui_ImplSDL3_InitForOpenGL(window, SDL_GL_GetCurrentContext());
        ImGui_ImplOpenGL3_Init("#version 410");
    }

    void ImGuiLayer::OnDetach()
    {
        ZoneScoped;

        ImGui:ImGui_ImplOpenGL3_Shutdown();
        ImGui_ImplSDL3_Shutdown();
        ImGui::DestroyContext();
    }

    void ImGuiLayer::OnEvent(Event& e)
    {
        if (m_BlockEvents)
		{
			ImGuiIO& io = ImGui::GetIO();
			e.Handled |= e.IsInCategory(EventCategoryMouse) & io.WantCaptureMouse;
			e.Handled |= e.IsInCategory(EventCategoryKeyboard) & io.WantCaptureKeyboard;
		}
    }

    void ImGuiLayer::Begin()
	{
        ZoneScoped;

		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplSDL3_NewFrame();
		ImGui::NewFrame();
	}

	void ImGuiLayer::End()
	{
        ZoneScoped;

		ImGuiIO& io = ImGui::GetIO();
		Application& app = Application::Get();
		io.DisplaySize = ImVec2((float)app.GetWindow().GetWidth(), (float)app.GetWindow().GetHeight());

		// Rendering
		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

/*       	if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable) //Comment this for disable the detached imgui windows from the main window
		{
            SDL_Window* backup_current_window = SDL_GL_GetCurrentWindow();
            SDL_GLContext backup_current_context = SDL_GL_GetCurrentContext();
            ImGui::UpdatePlatformWindows();
            ImGui::RenderPlatformWindowsDefault();
            SDL_GL_MakeCurrent(backup_current_window, backup_current_context);
		} */
	}

    void ImGuiLayer::OnImGuiRender()
    {
        static bool show = true;
        ImGui::ShowDemoWindow(&show);
    }

	void ImGuiLayer::ProcessEvents(const SDL_Event& event)
	{
		ImGui_ImplSDL3_ProcessEvent(&event);
	}

	void ImGuiLayer::SetTeaColorStyle()
	{
		ImGuiStyle& style = ImGui::GetStyle();
		style.FrameRounding = 1;
		style.WindowRounding = 2;
		style.GrabRounding = 1;
		style.WindowTitleAlign.x = 0.5;
		style.WindowMenuButtonPosition = ImGuiDir_None;

		//COFFEE THEME V2
		ImVec4* colors = ImGui::GetStyle().Colors;
		colors[ImGuiCol_Text] 					= ImVec4(0.92f, 0.93f, 0.95f, 1.00f);
		colors[ImGuiCol_TextDisabled]           = ImVec4(0.39f, 0.43f, 0.40f, 1.00f);
		colors[ImGuiCol_WindowBg]               = ImVec4(0.07f, 0.10f, 0.08f, 1.00f);
		colors[ImGuiCol_PopupBg]                = ImVec4(0.07f, 0.10f, 0.07f, 1.00f);
		colors[ImGuiCol_Border]                 = ImVec4(0.36f, 0.42f, 0.37f, 0.50f);
		colors[ImGuiCol_FrameBg]                = ImVec4(0.15f, 0.20f, 0.15f, 1.00f);
		colors[ImGuiCol_FrameBgHovered]         = ImVec4(0.20f, 0.25f, 0.20f, 1.00f);
		colors[ImGuiCol_FrameBgActive]          = ImVec4(0.23f, 0.29f, 0.23f, 1.00f);
		colors[ImGuiCol_TitleBg]                = ImVec4(0.10f, 0.16f, 0.12f, 1.00f);
		colors[ImGuiCol_TitleBgActive]          = ImVec4(0.15f, 0.22f, 0.16f, 1.00f);
		colors[ImGuiCol_TitleBgCollapsed]       = ImVec4(0.08f, 0.12f, 0.09f, 1.00f);
		colors[ImGuiCol_ScrollbarBg]            = ImVec4(0.18f, 0.21f, 0.18f, 1.00f);
		colors[ImGuiCol_ScrollbarGrab]          = ImVec4(0.35f, 0.38f, 0.35f, 1.00f);
		colors[ImGuiCol_ScrollbarGrabHovered]   = ImVec4(0.53f, 0.57f, 0.53f, 1.00f);
		colors[ImGuiCol_ScrollbarGrabActive]    = ImVec4(0.41f, 0.44f, 0.41f, 1.00f);
		colors[ImGuiCol_CheckMark]              = ImVec4(0.44f, 0.65f, 0.35f, 1.00f);
		colors[ImGuiCol_SliderGrab]             = ImVec4(0.44f, 0.65f, 0.35f, 1.00f);
		colors[ImGuiCol_SliderGrabActive]       = ImVec4(0.50f, 0.76f, 0.38f, 1.00f);
		colors[ImGuiCol_Button]                 = ImVec4(0.15f, 0.21f, 0.16f, 1.00f);
		colors[ImGuiCol_ButtonHovered]          = ImVec4(0.30f, 0.40f, 0.31f, 1.00f);
		colors[ImGuiCol_ButtonActive]           = ImVec4(0.18f, 0.32f, 0.19f, 1.00f);
		colors[ImGuiCol_Header]                 = ImVec4(0.15f, 0.22f, 0.16f, 1.00f);
		colors[ImGuiCol_HeaderHovered]          = ImVec4(0.22f, 0.33f, 0.22f, 1.00f);
		colors[ImGuiCol_HeaderActive]           = ImVec4(0.16f, 0.27f, 0.17f, 1.00f);
		colors[ImGuiCol_Separator]              = ImVec4(0.36f, 0.42f, 0.37f, 0.50f);
		colors[ImGuiCol_SeparatorHovered]       = ImVec4(0.51f, 0.62f, 0.53f, 0.50f);
		colors[ImGuiCol_SeparatorActive]        = ImVec4(0.62f, 0.75f, 0.64f, 0.50f);
		colors[ImGuiCol_MenuBarBg]              = ImVec4(0.06f, 0.08f, 0.06f, 1.00f);
		colors[ImGuiCol_ResizeGrip]             = ImVec4(0.41f, 0.44f, 0.41f, 1.00f);
		colors[ImGuiCol_ResizeGripHovered]      = ImVec4(0.53f, 0.57f, 0.54f, 1.00f);
		colors[ImGuiCol_ResizeGripActive]       = ImVec4(0.71f, 0.77f, 0.71f, 1.00f);
		colors[ImGuiCol_TabHovered]             = ImVec4(0.12f, 0.18f, 0.13f, 1.00f);
		colors[ImGuiCol_Tab]                    = ImVec4(0.14f, 0.20f, 0.15f, 1.00f);
		colors[ImGuiCol_TabDimmedSelected]      = ImVec4(0.21f, 0.29f, 0.21f, 1.00f);
		colors[ImGuiCol_TabSelected]            = ImVec4(0.15f, 0.21f, 0.16f, 1.00f);
		colors[ImGuiCol_TabDimmed]              = ImVec4(0.03f, 0.04f, 0.03f, 1.00f);
		colors[ImGuiCol_TabDimmedSelected]      = ImVec4(0.07f, 0.10f, 0.07f, 1.00f);
		colors[ImGuiCol_DockingEmptyBg]         = ImVec4(0.06f, 0.08f, 0.06f, 1.00f);
		colors[ImGuiCol_DockingPreview]         = ImVec4(0.23f, 0.48f, 0.26f, 0.70f);

		/* GODOT THEME GREEN
		ImVec4* colors = ImGui::GetStyle().Colors;
		colors[ImGuiCol_TextDisabled]           = ImVec4(0.39f, 0.43f, 0.40f, 1.00f);
		colors[ImGuiCol_WindowBg]               = ImVec4(0.13f, 0.18f, 0.14f, 1.00f);
		colors[ImGuiCol_PopupBg]                = ImVec4(0.21f, 0.29f, 0.22f, 1.00f);
		colors[ImGuiCol_FrameBg]                = ImVec4(0.11f, 0.16f, 0.12f, 1.00f);
		colors[ImGuiCol_FrameBgHovered]         = ImVec4(0.15f, 0.21f, 0.16f, 1.00f);
		colors[ImGuiCol_FrameBgActive]          = ImVec4(0.22f, 0.27f, 0.23f, 1.00f);
		colors[ImGuiCol_TitleBg]                = ImVec4(0.14f, 0.20f, 0.15f, 1.00f);
		colors[ImGuiCol_TitleBgActive]          = ImVec4(0.21f, 0.29f, 0.22f, 1.00f);
		colors[ImGuiCol_TitleBgCollapsed]       = ImVec4(0.14f, 0.20f, 0.15f, 1.00f);
		colors[ImGuiCol_ScrollbarBg]            = ImVec4(0.24f, 0.29f, 0.25f, 1.00f);
		colors[ImGuiCol_ScrollbarGrab]          = ImVec4(0.42f, 0.46f, 0.42f, 1.00f);
		colors[ImGuiCol_ScrollbarGrabHovered]   = ImVec4(0.60f, 0.65f, 0.61f, 1.00f);
		colors[ImGuiCol_ScrollbarGrabActive]    = ImVec4(0.48f, 0.52f, 0.49f, 1.00f);
		colors[ImGuiCol_CheckMark]              = ImVec4(0.49f, 0.73f, 0.39f, 1.00f);
		colors[ImGuiCol_SliderGrab]             = ImVec4(0.49f, 0.73f, 0.39f, 1.00f);
		colors[ImGuiCol_SliderGrabActive]       = ImVec4(0.56f, 0.84f, 0.42f, 1.00f);
		colors[ImGuiCol_Button]                 = ImVec4(0.21f, 0.29f, 0.22f, 1.00f);
		colors[ImGuiCol_ButtonHovered]          = ImVec4(0.36f, 0.48f, 0.37f, 1.00f);
		colors[ImGuiCol_ButtonActive]           = ImVec4(0.22f, 0.40f, 0.24f, 1.00f);
		colors[ImGuiCol_Header]                 = ImVec4(0.15f, 0.21f, 0.16f, 1.00f);
		colors[ImGuiCol_HeaderHovered]          = ImVec4(0.22f, 0.27f, 0.23f, 1.00f);
		colors[ImGuiCol_HeaderActive]           = ImVec4(0.15f, 0.21f, 0.16f, 1.00f);
		colors[ImGuiCol_Separator]              = ImVec4(0.43f, 0.50f, 0.44f, 0.50f);
		colors[ImGuiCol_TabHovered]             = ImVec4(0.17f, 0.24f, 0.18f, 1.00f);
		colors[ImGuiCol_Tab]                    = ImVec4(0.14f, 0.20f, 0.15f, 1.00f);
		colors[ImGuiCol_TabSelected]            = ImVec4(0.21f, 0.29f, 0.22f, 1.00f);
		colors[ImGuiCol_DockingEmptyBg]         = ImVec4(0.11f, 0.16f, 0.12f, 1.00f); */
	}

	void ImGuiLayer::SetGodotColorStyle()
	{
    	//GODOT THEME
    	ImVec4* colors = ImGui::GetStyle().Colors;
    	colors[ImGuiCol_Text]                   = ImVec4(0.92f, 0.93f, 0.95f, 1.00f);
    	colors[ImGuiCol_TextDisabled]           = ImVec4(0.39f, 0.40f, 0.43f, 1.00f);
    	colors[ImGuiCol_WindowBg]               = ImVec4(0.13f, 0.15f, 0.18f, 1.00f);
    	colors[ImGuiCol_PopupBg]                = ImVec4(0.21f, 0.24f, 0.29f, 1.00f);
    	colors[ImGuiCol_FrameBg]                = ImVec4(0.11f, 0.13f, 0.16f, 1.00f);
    	colors[ImGuiCol_FrameBgHovered]         = ImVec4(0.15f, 0.18f, 0.21f, 1.00f);
    	colors[ImGuiCol_FrameBgActive]          = ImVec4(0.22f, 0.24f, 0.27f, 1.00f);
    	colors[ImGuiCol_TitleBg]                = ImVec4(0.14f, 0.16f, 0.20f, 1.00f);
    	colors[ImGuiCol_TitleBgActive]          = ImVec4(0.21f, 0.24f, 0.29f, 1.00f);
    	colors[ImGuiCol_TitleBgCollapsed]       = ImVec4(0.14f, 0.16f, 0.20f, 1.00f);
    	colors[ImGuiCol_ScrollbarBg]            = ImVec4(0.24f, 0.26f, 0.29f, 1.00f);
    	colors[ImGuiCol_ScrollbarGrab]          = ImVec4(0.42f, 0.44f, 0.46f, 1.00f);
    	colors[ImGuiCol_ScrollbarGrabHovered]   = ImVec4(0.60f, 0.61f, 0.65f, 1.00f);
    	colors[ImGuiCol_ScrollbarGrabActive]    = ImVec4(0.48f, 0.49f, 0.52f, 1.00f);
    	colors[ImGuiCol_Button]                 = ImVec4(0.21f, 0.24f, 0.29f, 1.00f);
    	colors[ImGuiCol_ButtonHovered]          = ImVec4(0.36f, 0.40f, 0.48f, 1.00f);
    	colors[ImGuiCol_ButtonActive]           = ImVec4(0.22f, 0.31f, 0.40f, 1.00f);
    	colors[ImGuiCol_Header]                 = ImVec4(0.15f, 0.18f, 0.21f, 1.00f);
    	colors[ImGuiCol_HeaderHovered]          = ImVec4(0.22f, 0.24f, 0.27f, 1.00f);
    	colors[ImGuiCol_HeaderActive]           = ImVec4(0.15f, 0.18f, 0.21f, 1.00f);
    	colors[ImGuiCol_TabHovered]             = ImVec4(0.17f, 0.20f, 0.24f, 1.00f);
    	colors[ImGuiCol_Tab]                    = ImVec4(0.14f, 0.16f, 0.20f, 1.00f);
    	colors[ImGuiCol_TabSelected]            = ImVec4(0.21f, 0.24f, 0.29f, 1.00f);
    	colors[ImGuiCol_DockingEmptyBg]         = ImVec4(0.11f, 0.13f, 0.16f, 1.00f);
	}

    void ImGuiLayer::SetCoffeeColorStyle()
    {
        //COFFEE THEME V1
		ImVec4* colors = ImGui::GetStyle().Colors;
		colors[ImGuiCol_Text] = ImColor{IM_COL32(0xeb, 0xdb, 0xb2, 0xFF)};
        colors[ImGuiCol_TextDisabled] = ImColor{IM_COL32(0x92, 0x83, 0x74, 0xFF)};
        colors[ImGuiCol_WindowBg] = ImColor{IM_COL32(0x1d, 0x20, 0x21, 0xF0)};
        colors[ImGuiCol_ChildBg] = ImColor{IM_COL32(0x1d, 0x20, 0x21, 0xFF)};
        colors[ImGuiCol_PopupBg] = ImColor{IM_COL32(0x1d, 0x20, 0x21, 0xF0)};
        colors[ImGuiCol_Border] = ImColor{IM_COL32(0x1d, 0x20, 0x21, 0xFF)};
        colors[ImGuiCol_BorderShadow] = ImColor{0};
        colors[ImGuiCol_FrameBg] = ImColor{IM_COL32(0x3c, 0x38, 0x36, 0x90)};
        colors[ImGuiCol_FrameBgHovered] = ImColor{IM_COL32(0x50, 0x49, 0x45, 0xFF)};
        colors[ImGuiCol_FrameBgActive] = ImColor{IM_COL32(0x66, 0x5c, 0x54, 0xA8)};
        colors[ImGuiCol_TitleBg] = ImColor{IM_COL32(0xd6, 0x5d, 0x0e, 0xFF)};
        colors[ImGuiCol_TitleBgActive] = ImColor{IM_COL32(0xfe, 0x80, 0x19, 0xFF)};
        colors[ImGuiCol_TitleBgCollapsed] = ImColor{IM_COL32(0xd6, 0x5d, 0x0e, 0x9C)};
        colors[ImGuiCol_MenuBarBg] = ImColor{IM_COL32(0x28, 0x28, 0x28, 0xF0)};
        colors[ImGuiCol_ScrollbarBg] = ImColor{IM_COL32(0x00, 0x00, 0x00, 0x28)};
        colors[ImGuiCol_ScrollbarGrab] = ImColor{IM_COL32(0x3c, 0x38, 0x36, 0xFF)};
        colors[ImGuiCol_ScrollbarGrabHovered] = ImColor{IM_COL32(0x50, 0x49, 0x45, 0xFF)};
        colors[ImGuiCol_ScrollbarGrabActive] = ImColor{IM_COL32(0x66, 0x5c, 0x54, 0xFF)};
        colors[ImGuiCol_CheckMark] = ImColor{IM_COL32(0xd6, 0x5d, 0x0e, 0x9E)};
        colors[ImGuiCol_SliderGrab] = ImColor{IM_COL32(0xd6, 0x5d, 0x0e, 0x70)};
        colors[ImGuiCol_SliderGrabActive] = ImColor{IM_COL32(0xfe, 0x80, 0x19, 0xFF)};
        colors[ImGuiCol_Button] = ImColor{IM_COL32(0xd6, 0x5d, 0x0e, 0x66)};
        colors[ImGuiCol_ButtonHovered] = ImColor{IM_COL32(0xfe, 0x80, 0x19, 0x9E)};
        colors[ImGuiCol_ButtonActive] = ImColor{IM_COL32(0xfe, 0x80, 0x19, 0xFF)};
        colors[ImGuiCol_Header] = ImColor{IM_COL32(0xd6, 0x5d, 0x0e, 0.4F)};
        colors[ImGuiCol_HeaderHovered] = ImColor{IM_COL32(0xfe, 0x80, 0x19, 0xCC)};
        colors[ImGuiCol_HeaderActive] = ImColor{IM_COL32(0xfe, 0x80, 0x19, 0xFF)};
        colors[ImGuiCol_Separator] = ImColor{IM_COL32(0x66, 0x5c, 0x54, 0.50f)};
        colors[ImGuiCol_SeparatorHovered] = ImColor{IM_COL32(0x50, 0x49, 0x45, 0.78f)};
        colors[ImGuiCol_SeparatorActive] = ImColor{IM_COL32(0x66, 0x5c, 0x54, 0xFF)};
        colors[ImGuiCol_ResizeGrip] = ImColor{IM_COL32(0xd6, 0x5d, 0x0e, 0x40)};
        colors[ImGuiCol_ResizeGripHovered] = ImColor{IM_COL32(0xfe, 0x80, 0x19, 0xAA)};
        colors[ImGuiCol_ResizeGripActive] = ImColor{IM_COL32(0xfe, 0x80, 0x19, 0xF2)};
        colors[ImGuiCol_Tab] = ImColor{IM_COL32(0xd6, 0x5d, 0x0e, 0xD8)};
        colors[ImGuiCol_TabHovered] = ImColor{IM_COL32(0xfe, 0x80, 0x19, 0xCC)};
        colors[ImGuiCol_TabActive] = ImColor{IM_COL32(0xfe, 0x80, 0x19, 0xFF)};
        colors[ImGuiCol_TabUnfocused] = ImColor{IM_COL32(0x1d, 0x20, 0x21, 0.97f)};
        colors[ImGuiCol_TabUnfocusedActive] = ImColor{IM_COL32(0x1d, 0x20, 0x21, 0xFF)};
        colors[ImGuiCol_PlotLines] = ImColor{IM_COL32(0xd6, 0x5d, 0x0e, 0xFF)};
        colors[ImGuiCol_PlotLinesHovered] = ImColor{IM_COL32(0xfe, 0x80, 0x19, 0xFF)};
        colors[ImGuiCol_PlotHistogram] = ImColor{IM_COL32(0x98, 0x97, 0x1a, 0xFF)};
        colors[ImGuiCol_PlotHistogramHovered] = ImColor{IM_COL32(0xb8, 0xbb, 0x26, 0xFF)};
        colors[ImGuiCol_TextSelectedBg] = ImColor{IM_COL32(0x45, 0x85, 0x88, 0x59)};
        colors[ImGuiCol_DragDropTarget] = ImColor{IM_COL32(0x98, 0x97, 0x1a, 0.90f)};
        colors[ImGuiCol_TableHeaderBg] = ImColor{IM_COL32(0x38, 0x3c, 0x36, 0xFF)};
        colors[ImGuiCol_TableBorderStrong] = ImColor{IM_COL32(0x28, 0x28, 0x28, 0xFF)};
        colors[ImGuiCol_TableBorderLight] = ImColor{IM_COL32(0x38, 0x3c, 0x36, 0xFF)};
        colors[ImGuiCol_TableRowBg] = ImColor {IM_COL32(0x1d, 0x20, 0x21, 0xFF)};
        colors[ImGuiCol_TableRowBgAlt] = ImColor{IM_COL32(0x28, 0x28, 0x28, 0xFF)};
        colors[ImGuiCol_TextSelectedBg] = ImColor { IM_COL32(0x45, 0x85, 0x88, 0xF0) };
        colors[ImGuiCol_NavHighlight] = ImColor{IM_COL32(0x83, 0xa5, 0x98, 0xFF)};
        colors[ImGuiCol_NavWindowingHighlight] = ImColor{IM_COL32(0xfb, 0xf1, 0xc7, 0xB2)};
        colors[ImGuiCol_NavWindowingDimBg] = ImColor{IM_COL32(0x7c, 0x6f, 0x64, 0x33)};
        colors[ImGuiCol_ModalWindowDimBg] = ImColor{IM_COL32(0x1d, 0x20, 0x21, 0x59)};
    }
}
