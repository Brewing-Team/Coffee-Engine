#pragma once
#include <entt/entity/registry.hpp>
#include "UIAnchor.h"

namespace Coffee {

    class UIManager
    {
    public:
        enum class UIComponentType
        {
            Image,
            Text,
            Toggle,
            Button
        };

        struct UIRenderItem {
            entt::entity Entity;
            int Layer;
            UIComponentType ComponentType;
            entt::entity Parent;
            entt::entity Next;
        };

        static void UpdateUI(entt::registry& registry);

        static void MarkForSorting() { s_NeedsSorting = true; }

        static AnchorPreset GetAnchorPreset(int row, int column);

    private:
        static void SortUIElements(entt::registry& registry);

        static void RenderUIImage(entt::registry& registry, entt::entity entity);
        static void RenderUIText(entt::registry& registry, entt::entity entity);
        static void RenderUIToggle(entt::registry& registry, entt::entity entity);
        static void RenderUIButton(entt::registry& registry, entt::entity entity);

    public:
        static glm::vec2 WindowSize;

    private:
        static bool s_NeedsSorting;
        static std::vector<UIRenderItem> s_SortedUIItems;
    };

} // Coffee