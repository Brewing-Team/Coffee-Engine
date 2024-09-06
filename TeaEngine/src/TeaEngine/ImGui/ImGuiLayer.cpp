#include "ImGuiLayer.h"
#include "TeaEngine/Core/Application.h"
#include "TeaEngine/Core/Window.h"

#include <imgui.h>
#include <imgui_internal.h>

#include <backends/imgui_impl_glfw.h>
#include <backends/imgui_impl_opengl3.h>
#include <GLFW/glfw3.h>
#include <tracy/Tracy.hpp>

namespace Tea {

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

        ImGui::CreateContext();
        ImGui::StyleColorsDark();

        ImGuiIO& io = ImGui::GetIO();
        io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
        io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
        //io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable; //Comment this for disable the detached imgui windows from the main window

        float fontSize = 18.0f;
        io.FontDefault = io.Fonts->AddFontFromFileTTF("/usr/share/fonts/TTF/OpenSans-SemiBold.ttf", fontSize);

		/* float fontSize = 16.0f;
        io.FontDefault = io.Fonts->AddFontFromFileTTF("/home/hugo/.fonts/iosevka-nerd-font.ttf", fontSize); */

		SetTeaColorStyle();

        Application& app = Application::Get();
        GLFWwindow* window = static_cast<GLFWwindow*>(app.GetWindow().GetNativeWindow());

        ImGui_ImplGlfw_InitForOpenGL(window, true);
        ImGui_ImplOpenGL3_Init("#version 410");
    }

    void ImGuiLayer::OnDetach()
    {
        ZoneScoped;

        ImGui:ImGui_ImplOpenGL3_Shutdown();
        ImGui_ImplGlfw_Shutdown();
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
		ImGui_ImplGlfw_NewFrame();
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

      /*   if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable) //Comment this for disable the detached imgui windows from the main window
		{
			GLFWwindow* backup_current_context = glfwGetCurrentContext();
			ImGui::UpdatePlatformWindows();
			ImGui::RenderPlatformWindowsDefault();
			glfwMakeContextCurrent(backup_current_context);
		} */
	}

    void ImGuiLayer::OnImGuiRender()
    {
        static bool show = true;
        ImGui::ShowDemoWindow(&show);
    }

	void ImGuiLayer::SetTeaColorStyle()
	{
		ImGuiStyle& style = ImGui::GetStyle();
		style.FrameRounding = 1;
		style.WindowRounding = 2;
		style.GrabRounding = 1;
		style.WindowTitleAlign.x = 0.5;
		style.WindowMenuButtonPosition = ImGuiDir_None;

		//TEA THEME V2
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







		/* //TEA THEME V1
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
		colors[ImGuiCol_SeparatorHovered]       = ImVec4(0.58f, 0.70f, 0.59f, 0.50f);
		colors[ImGuiCol_SeparatorActive]        = ImVec4(0.69f, 0.83f, 0.70f, 0.50f);
		colors[ImGuiCol_ResizeGrip]             = ImVec4(0.42f, 0.46f, 0.42f, 1.00f);
		colors[ImGuiCol_ResizeGripHovered]      = ImVec4(0.60f, 0.65f, 0.61f, 1.00f);
		colors[ImGuiCol_ResizeGripActive]       = ImVec4(0.48f, 0.52f, 0.49f, 1.00f);
		colors[ImGuiCol_TabHovered]             = ImVec4(0.17f, 0.24f, 0.18f, 1.00f);
		colors[ImGuiCol_Tab]                    = ImVec4(0.14f, 0.20f, 0.15f, 1.00f);
		colors[ImGuiCol_TabSelected]            = ImVec4(0.21f, 0.29f, 0.22f, 1.00f);
		colors[ImGuiCol_DockingEmptyBg]         = ImVec4(0.11f, 0.16f, 0.12f, 1.00f); */

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
}
