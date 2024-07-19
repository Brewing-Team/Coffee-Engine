#include "EditorLayer.h"
#include "TeaEngine/Core/Base.h"
#include "TeaEngine/Core/Log.h"
#include "TeaEngine/Renderer/EditorCamera.h"
#include "TeaEngine/Renderer/RendererAPI.h"
#include "TeaEngine/Scene/Scene.h"
#include "imgui.h"

namespace Tea {

    EditorLayer::EditorLayer() : Layer("Example")
    {
      
    }

    void EditorLayer::OnAttach()
    {
        m_EditorScene = CreateRef<Scene>();
        m_ActiveScene = m_EditorScene;

        m_EditorCamera = EditorCamera(30.0f);

        m_ActiveScene->OnInit();
    }

    void EditorLayer::OnUpdate()
    {
        RendererAPI::SetClearColor({.2f,.2f,.2f,1});
        RendererAPI::Clear();

        m_ActiveScene->OnUpdate();
    }

    void EditorLayer::OnEvent(Tea::Event& event)
    {
        m_ActiveScene->OnEvent(event);
    }

    void EditorLayer::OnDetach()
    {
        m_ActiveScene->OnExit();
    }

    void EditorLayer::OnImGuiRender()
    {
        ImGui::DockSpaceOverViewport(0, ImGui::GetMainViewport());
        ImGui::BeginMainMenuBar();
        ImGui::EndMainMenuBar();
    }

}
