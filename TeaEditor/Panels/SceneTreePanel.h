#pragma once

#include "TeaEngine/Core/Base.h"
#include "TeaEngine/Scene/Scene.h"
#include "TeaEngine/Scene/Entity.h"
#include "entt/entity/fwd.hpp"

namespace Tea {

    class SceneTreePanel
    {
    public:
        SceneTreePanel() = default;
        SceneTreePanel(const Ref<Scene>& scene);

        void SetContext(const Ref<Scene>& scene);

        void OnImGuiRender();
    private:
        void DrawEntityNode(Entity entity);
        void DrawComponents(Entity entity);
    private:
        Ref<Scene> m_Context;
        Entity m_SelectionContext;
    };

}