#pragma once

#include "Panels/Panel.h"
#include "TeaEngine/Core/Base.h"
#include "TeaEngine/Core/Layer.h"
#include "TeaEngine/Events/KeyEvent.h"
#include "TeaEngine/Renderer/EditorCamera.h"
#include "TeaEngine/Renderer/Framebuffer.h"
#include "TeaEngine/Scene/Scene.h"
#include "Panels/ContentBrowserPanel.h"
#include "Panels/SceneTreePanel.h"

namespace Tea {

    class EditorLayer : public Tea::Layer
    {
    public:
        EditorLayer();
        virtual ~EditorLayer() = default;

        void OnAttach() override;

        void OnUpdate(float dt) override;

        void OnEvent(Event& event) override;

        bool OnKeyPressed(KeyPressedEvent& event);

        void OnDetach() override;

        void OnImGuiRender() override;
    private:
        void OnOverlayRender();
        void ResizeViewport(float width, float height);

        //Project Management
        void NewProject();
        void OpenProject();
        void SaveProject();
        void SaveProjectAs();

        //Scene Management
        void NewScene();
        void OpenScene();
        void SaveScene();
        void SaveSceneAs();
    private:
        Ref<Scene> m_EditorScene;
        Ref<Scene> m_ActiveScene;

        EditorCamera m_EditorCamera;

        enum class SceneState
        {
            Edit = 0,
            Play = 1
        }m_SceneState = SceneState::Edit;

        bool m_ViewportFocused = false, m_ViewportHovered = false;
		glm::vec2 m_ViewportSize = { 0.0f, 0.0f };
        glm::vec2 m_ViewportBounds[2];

        int m_GizmoType = -1;

        //Panels
        SceneTreePanel m_SceneTreePanel;
        ContentBrowserPanel m_ContentBrowserPanel;
    };

}
