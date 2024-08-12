#include "EditorLayer.h"
#include "TeaEngine/Core/Base.h"
#include "TeaEngine/Core/Log.h"
#include "TeaEngine/Core/Application.h"
#include "TeaEngine/Renderer/EditorCamera.h"
#include "TeaEngine/Renderer/RendererAPI.h"
#include "TeaEngine/Scene/Scene.h"
#include "Panels/SceneTreePanel.h"
#include "imgui.h"
#include <cstdint>
#include <sys/types.h>
#include <tracy/Tracy.hpp>

namespace Tea {

    EditorLayer::EditorLayer() : Layer("Example")
    {

    }

    void EditorLayer::OnAttach()
    {
        ZoneScoped;

        m_Framebuffer = Framebuffer::Create(1280, 720, {ImageFormat::RGBA8, ImageFormat::DEPTH24STENCIL8});

        m_EditorScene = CreateRef<Scene>();
        m_ActiveScene = m_EditorScene;

        m_EditorCamera = EditorCamera(45.0f);

        m_ActiveScene->OnInit();

        m_SceneTreePanel.SetContext(m_ActiveScene);
    }

    void EditorLayer::OnUpdate(float dt)
    {
        ZoneScoped;

        if((m_ViewportSize.x > 0.0f && m_ViewportSize.y > 0.0f) &&
           (m_Framebuffer->GetWidth() != m_ViewportSize.x || m_Framebuffer->GetHeight() != m_ViewportSize.y))
        {
            m_Framebuffer->Resize((uint32_t)m_ViewportSize.x,(uint32_t)m_ViewportSize.y);
            m_EditorCamera.SetViewportSize(m_ViewportSize.x, m_ViewportSize.y);
        }

        m_Framebuffer->Bind();
        RendererAPI::SetClearColor({.1f,.1f,.1f,1});
        RendererAPI::Clear();

        m_EditorCamera.OnUpdate();

        m_ActiveScene->OnUpdate();
        m_ActiveScene->OnUpdateEditor(m_EditorCamera);

        m_Framebuffer->UnBind();
    }

    void EditorLayer::OnEvent(Tea::Event& event)
    {
        ZoneScoped;

        m_EditorCamera.OnEvent(event);

        m_ActiveScene->OnEvent(event);
    }

    void EditorLayer::OnDetach()
    {
        ZoneScoped;

        m_ActiveScene->OnExit();
    }

    void EditorLayer::OnImGuiRender()
    {
        ZoneScoped;

        ImGui::DockSpaceOverViewport(0, ImGui::GetMainViewport());

        if (ImGui::BeginMainMenuBar()) {

            if (ImGui::BeginMenu("File")) {
                if (ImGui::MenuItem("Open Project...", "Ctrl+O")) { }
                if (ImGui::MenuItem("Exit")) { Application::Get().Close(); }

                ImGui::EndMenu();
            }
            
            ImGui::SetCursorPosX(ImGui::GetContentRegionAvail().x - 25);
            ImGui::TextDisabled("FPS: %.1f", ImGui::GetIO().Framerate);

            ImGui::EndMainMenuBar();
        }

        m_SceneTreePanel.OnImGuiRender();

        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));
        ImGui::Begin("Viewport");

        m_ViewportFocused = ImGui::IsWindowFocused();
        m_ViewportHovered = ImGui::IsWindowHovered();

        Application::Get().GetImGuiLayer()->BlockEvents(!m_ViewportHovered);

        ImVec2 viewportPanelSize = ImGui::GetContentRegionAvail();
        m_ViewportSize = { viewportPanelSize.x, viewportPanelSize.y };

        uint32_t textureID = m_Framebuffer->GetColorAttachmentID();
        ImGui::Image((void*)textureID, ImVec2{ m_ViewportSize.x, m_ViewportSize.y }, {0, 1}, {1, 0});
        ImGui::End();
        ImGui::PopStyleVar();
    }

}
