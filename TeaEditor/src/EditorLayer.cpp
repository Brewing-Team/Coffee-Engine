#include "EditorLayer.h"
#include "TeaEngine/Core/Base.h"
#include "TeaEngine/Core/Log.h"
#include "TeaEngine/Core/Application.h"
#include "TeaEngine/Renderer/EditorCamera.h"
#include "TeaEngine/Renderer/RendererAPI.h"
#include "TeaEngine/Scene/Components.h"
#include "TeaEngine/Scene/Scene.h"
#include "Panels/SceneTreePanel.h"
#include <glm/fwd.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <imgui.h>
#include <ImGuizmo.h>
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

        //REMOVE!!!
        m_GizmoType = ImGuizmo::OPERATION::TRANSLATE;
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

            if (ImGui::BeginMenu("File"))
            {
                if (ImGui::MenuItem("Open Project...", "Ctrl+O")) { }
                if (ImGui::MenuItem("Exit")) { Application::Get().Close(); }

                ImGui::EndMenu();
            }
            if (ImGui::BeginMenu("Project"))
            {
                ImGui::EndMenu();
            }
            if (ImGui::BeginMenu("Editor"))
            {
                ImGui::EndMenu();
            }
            if (ImGui::BeginMenu("About"))
            {
                if(ImGui::MenuItem("About TeaEngine"))
                { 
                    ImGui::OpenPopup("About TeaEngine");
                }
                ImGui::EndMenu();
            }

            ImGui::SetCursorPosX(ImGui::GetContentRegionAvail().x - 25);
            ImGui::TextDisabled("FPS: %.1f", ImGui::GetIO().Framerate);

            ImGui::EndMainMenuBar();
        }


        //TODO: Move this to a separate function
        if(ImGui::BeginPopupModal("About TeaEngine"))
        {
            ImGui::Text("TeaEngine is a game engine developed by Hugo Planell Moreno.");
            ImGui::Text("This engine is being developed as part of a personal project.");
            ImGui::Text("The engine is still in development and is not ready for production.");
            ImGui::Text("The engine is open source and can be found on GitHub.");
            ImGui::Text("The engine is licensed under the MIT License.");
            if (ImGui::Button("Close"))
                ImGui::CloseCurrentPopup();
            ImGui::EndPopup();
        }

        m_SceneTreePanel.OnImGuiRender();

        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));
        ImGui::Begin("Viewport");
        auto viewportMinRegion = ImGui::GetWindowContentRegionMin();
		auto viewportMaxRegion = ImGui::GetWindowContentRegionMax();
		auto viewportOffset = ImGui::GetWindowPos();
		m_ViewportBounds[0] = { viewportMinRegion.x + viewportOffset.x, viewportMinRegion.y + viewportOffset.y };
		m_ViewportBounds[1] = { viewportMaxRegion.x + viewportOffset.x, viewportMaxRegion.y + viewportOffset.y };

        m_ViewportFocused = ImGui::IsWindowFocused();
        m_ViewportHovered = ImGui::IsWindowHovered();

        Application::Get().GetImGuiLayer()->BlockEvents(!m_ViewportHovered);

        ImVec2 viewportPanelSize = ImGui::GetContentRegionAvail();
        m_ViewportSize = { viewportPanelSize.x, viewportPanelSize.y };

        uint32_t textureID = m_Framebuffer->GetColorAttachmentID();
        ImGui::Image((void*)textureID, ImVec2{ m_ViewportSize.x, m_ViewportSize.y }, {0, 1}, {1, 0});

        //Guizmo
        Entity selectedEntity = m_SceneTreePanel.GetSelectedEntity();

        if(selectedEntity and m_GizmoType != -1)
        {
            ImGuizmo::SetOrthographic(false);
            ImGuizmo::SetDrawlist();

            ImGuizmo::SetRect(m_ViewportBounds[0].x, m_ViewportBounds[0].y, m_ViewportBounds[1].x - m_ViewportBounds[0].x, m_ViewportBounds[1].y - m_ViewportBounds[0].y);

            const glm::mat4& cameraProjection = m_EditorCamera.GetProjection();
            glm::mat4 cameraView = m_EditorCamera.GetViewMatrix();

            auto& transformComponent = selectedEntity.GetComponent<TransformComponent>();
            glm::mat4 transform = transformComponent.GetWorldTransform();

            ImGuizmo::Manipulate(glm::value_ptr(cameraView), glm::value_ptr(cameraProjection),
                                (ImGuizmo::OPERATION)m_GizmoType, ImGuizmo::LOCAL,
                         glm::value_ptr(transform));

            if(ImGuizmo::IsUsing())
            {
                glm::vec3 skew;
                glm::vec4 perspective;
                glm::quat orientation;

                glm::vec3 position, rotation, scale;

                glm::decompose(transform, scale, orientation, position, skew, perspective);
                rotation = glm::eulerAngles(orientation);

                transformComponent.Position = position;
                transformComponent.Rotation = rotation;
                transformComponent.Scale = scale;
            }
        }

        ImGui::End();
        ImGui::PopStyleVar();
    }

}
