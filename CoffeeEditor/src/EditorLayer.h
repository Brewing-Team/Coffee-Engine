#pragma once

#include "Panels/ContentBrowserPanel.h"
#include "Panels/Panel.h"
#include "Panels/SceneTreePanel.h"
#include "CoffeeEngine/Core/Base.h"
#include "CoffeeEngine/Core/Layer.h"
#include "CoffeeEngine/Events/KeyEvent.h"
#include "CoffeeEngine/Renderer/EditorCamera.h"
#include "CoffeeEngine/Renderer/Framebuffer.h"
#include "CoffeeEngine/Scene/Scene.h"

#include <Panels/OutputPanel.h>

namespace Coffee {

    class EditorLayer : public Coffee::Layer
    {
    public:
        EditorLayer();
        virtual ~EditorLayer() = default;

        void OnAttach() override;

        void OnUpdate(float dt) override;

        void OnEvent(Event& event) override;

        bool OnKeyPressed(KeyPressedEvent& event);
        bool OnMouseButtonPressed(MouseButtonPressedEvent& event);

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
        OutputPanel m_OutputPanel;
    };

}
