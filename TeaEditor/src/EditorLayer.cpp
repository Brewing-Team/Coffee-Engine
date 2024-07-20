#include "EditorLayer.h"
#include "TeaEngine/Core/Base.h"
#include "TeaEngine/Core/Log.h"
#include "TeaEngine/Core/Application.h"
#include "TeaEngine/Renderer/EditorCamera.h"
#include "TeaEngine/Renderer/RendererAPI.h"
#include "TeaEngine/Scene/Scene.h"
#include "Panels/SceneTreePanel.h"
#include "imgui.h"
#include <sys/types.h>

namespace Tea {

    EditorLayer::EditorLayer() : Layer("Example")
    {
      
    }

    void EditorLayer::OnAttach()
    {
        m_Framebuffer = Framebuffer::Create(1280, 720, {ImageFormat::RGBA8, ImageFormat::DEPTH24STENCIL8});

        m_EditorScene = CreateRef<Scene>();
        m_ActiveScene = m_EditorScene;

        m_EditorCamera = EditorCamera(45.0f);

        m_ActiveScene->OnInit();

        m_SceneTreePanel.SetContext(m_ActiveScene);
    }

    void EditorLayer::OnUpdate()
    {
        m_Framebuffer->Bind();
        RendererAPI::SetClearColor({.2f,.2f,.2f,1});
        RendererAPI::Clear();

        m_EditorCamera.OnUpdate();

        m_ActiveScene->OnUpdate();
        m_ActiveScene->OnUpdateEditor(m_EditorCamera);

        m_Framebuffer->UnBind();
    }

    void EditorLayer::OnEvent(Tea::Event& event)
    {
        m_EditorCamera.OnEvent(event);

        m_ActiveScene->OnEvent(event);
    }

    void EditorLayer::OnDetach()
    {
        m_ActiveScene->OnExit();
    }

    void EditorLayer::OnImGuiRender()
    {
        ImGui::DockSpaceOverViewport(0, ImGui::GetMainViewport());

        if (ImGui::BeginMainMenuBar()) {

            if (ImGui::BeginMenu("File")) {
                if (ImGui::MenuItem("Open Project...", "Ctrl+O")) { }
                if (ImGui::MenuItem("Exit")) { Application::Get().Close(); }

                ImGui::EndMenu();
            }
            ImGui::EndMainMenuBar();
        }

        m_SceneTreePanel.OnImGuiRender();

        ImGui::Begin("Viewport");
        uint32_t textureID = m_Framebuffer->GetColorAttachmentID();
        ImVec2 viewportPanelSize = ImGui::GetContentRegionAvail();
        ImGui::Image((void*)textureID, viewportPanelSize, {0, 1}, {1, 0});
        ImGui::End();
    }

}
