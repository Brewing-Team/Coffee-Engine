#pragma once

#include "CoffeeEngine/Core/Base.h"
#include "CoffeeEngine/Scene/Scene.h"
#include "CoffeeEngine/Scene/Entity.h"
#include "Panel.h"

namespace Coffee {

    class InspectorPanel : public Panel {

        public:
            InspectorPanel() = default;
            InspectorPanel(const Ref<Scene>& scene);

            void SetContext(const Ref<Scene>& scene);

            void OnImGuiRender() override;

            void SetSelectedEntity(Entity entity) { m_SelectionContext = entity; };

        private:
            Ref<Scene> m_Context;
            Entity m_SelectionContext;

            void DrawEntityNode(Entity entity);
            void DrawComponents(Entity entity);
    };

} // Coffee
