#pragma once

#include "TeaEngine/Core/Base.h"
#include "TeaEngine/Renderer/EditorCamera.h"
#include "TeaEngine/Scene/Scene.h"
#include "TeaEngine/Core/Layer.h"

namespace Tea {

    class EditorLayer : public Tea::Layer
    {
    public:
        EditorLayer();
        virtual ~EditorLayer() = default;

        void OnAttach() override;

        void OnUpdate() override;

        void OnEvent(Tea::Event& event) override;

        void OnDetach() override;

        void OnImGuiRender() override;
    private:
        Ref<Scene> m_EditorScene;
        Ref<Scene> m_ActiveScene;

        EditorCamera m_EditorCamera;
    };
    
}
