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
            Button,
            Slider
        };

        struct UIRenderItem {
            entt::entity Entity;
            int Layer;
            UIComponentType ComponentType;
            entt::entity Parent;
            entt::entity Next;
        };

        struct AnchoredTransform {
            glm::vec2 Position;
            glm::vec2 Size;
        };

        static void UpdateUI(entt::registry& registry);

        static void MarkForSorting() { s_NeedsSorting = true; }

        static AnchorPreset GetAnchorPreset(int row, int column);

        static glm::vec2 GetParentSize(entt::registry& registry, entt::entity parentEntity);

    private:
        static void SortUIElements(entt::registry& registry);

        static void RenderUIImage(entt::registry& registry, entt::entity entity);
        static void RenderUIText(entt::registry& registry, entt::entity entity);
        static void RenderUIToggle(entt::registry& registry, entt::entity entity);
        static void RenderUIButton(entt::registry& registry, entt::entity entity);
        static void RenderUISlider(entt::registry& registry, entt::entity entity);

        static AnchoredTransform CalculateAnchoredTransform(entt::registry& registry, entt::entity entity, const RectAnchor& anchor, const glm::vec2& windowSize);
    public:
        static glm::vec2 WindowSize;

    private:
        static bool s_NeedsSorting;
        static std::vector<UIRenderItem> s_SortedUIItems;
    };

} // Coffee