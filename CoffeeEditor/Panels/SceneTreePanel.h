#pragma once

#include "CoffeeEngine/Core/Base.h"
#include "CoffeeEngine/Scene/Scene.h"
#include "CoffeeEngine/Scene/Entity.h"
#include "Panel.h"
#include "entt/entity/fwd.hpp"

namespace Coffee {

    class SceneTreePanel : public Panel
    {
    public:
        SceneTreePanel() = default;
        SceneTreePanel(const Ref<Scene>& scene);

        void SetContext(const Ref<Scene>& scene);

        void OnImGuiRender() override;

        Entity GetSelectedEntity() const { return m_SelectionContext; };
        void SetSelectedEntity(Entity entity) { m_SelectionContext = entity; };

        void CreatePrefab(Entity entity);
        void InstantiatePrefab(const std::filesystem::path& path);

    private:
        void DrawEntityNode(Entity entity);
        void DrawComponents(Entity entity);

        //UI functions for scenetree menus
        void ShowCreateEntityMenu();
        bool ResizeColliderToFitMeshAABB(Entity entity, RigidbodyComponent& rbComponent);

    private:
        Ref<Scene> m_Context;
        Entity m_SelectionContext;
        std::vector<std::filesystem::path> m_RecentPrefabs;

        bool m_ShowLuaScriptOptions = false;
    };

}