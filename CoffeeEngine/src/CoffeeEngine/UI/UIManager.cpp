#include "UIManager.h"

#include "CoffeeEngine/Renderer/Renderer.h"
#include "CoffeeEngine/Renderer/Renderer2D.h"
#include "CoffeeEngine/Scene/Components.h"
#include "CoffeeEngine/Scene/SceneTree.h"

namespace Coffee {

    glm::vec2 UIManager::WindowSize;
    bool UIManager::s_NeedsSorting = true;
    std::vector<UIManager::UIRenderItem> UIManager::s_SortedUIItems;

    void UIManager::UpdateUI(entt::registry& registry)
    {
        WindowSize = Renderer::GetCurrentRenderTarget()->GetSize();

        if (s_NeedsSorting)
        {
            SortUIElements(registry);
            s_NeedsSorting = false;
        }

        for (const auto& item : s_SortedUIItems)
        {
            entt::entity entity = item.Entity;

            switch (item.ComponentType) {
            case UIComponentType::Image:
                RenderUIImage(registry, entity);
                break;
            case UIComponentType::Text:
                RenderUIText(registry, entity);
                break;
            case UIComponentType::Toggle:
                RenderUIToggle(registry, entity);
                break;
            case UIComponentType::Button:
                RenderUIButton(registry, entity);
                break;
            }
        }
    }

    void UIManager::SortUIElements(entt::registry& registry)
    {
        s_SortedUIItems.clear();

        auto uiImageView = registry.view<UIImageComponent, TransformComponent>();
        for (auto entity : uiImageView) {
            auto& uiComponent = uiImageView.get<UIImageComponent>(entity);

            UIRenderItem item;
            item.Entity = entity;
            item.Layer = uiComponent.Layer;
            item.ComponentType = UIComponentType::Image;

            if (registry.any_of<HierarchyComponent>(entity))
            {
                auto& hierarchy = registry.get<HierarchyComponent>(entity);
                item.Parent = hierarchy.m_Parent;
                item.Next = hierarchy.m_Next;
            }

            s_SortedUIItems.push_back(item);
        }

        auto uiTextView = registry.view<UITextComponent, TransformComponent>();
        for (auto entity : uiTextView)
        {
            auto& uiComponent = uiTextView.get<UITextComponent>(entity);

            UIRenderItem item;
            item.Entity = entity;
            item.Layer = uiComponent.Layer;
            item.ComponentType = UIComponentType::Text;

            if (registry.any_of<HierarchyComponent>(entity))
            {
                auto& hierarchy = registry.get<HierarchyComponent>(entity);
                item.Parent = hierarchy.m_Parent;
                item.Next = hierarchy.m_Next;
            }

            s_SortedUIItems.push_back(item);
        }

        auto uiToggleView = registry.view<UIToggleComponent, TransformComponent>();
        for (auto entity : uiToggleView)
        {
            auto& uiComponent = uiToggleView.get<UIToggleComponent>(entity);

            UIRenderItem item;
            item.Entity = entity;
            item.Layer = uiComponent.Layer;
            item.ComponentType = UIComponentType::Toggle;

            if (registry.any_of<HierarchyComponent>(entity))
            {
                auto& hierarchy = registry.get<HierarchyComponent>(entity);
                item.Parent = hierarchy.m_Parent;
                item.Next = hierarchy.m_Next;
            }

            s_SortedUIItems.push_back(item);
        }

        auto uiButtonView = registry.view<UIButtonComponent, TransformComponent>();
        for (auto entity : uiButtonView)
        {
            auto& uiComponent = uiButtonView.get<UIButtonComponent>(entity);

            UIRenderItem item;
            item.Entity = entity;
            item.Layer = uiComponent.Layer;
            item.ComponentType = UIComponentType::Button;

            if (registry.any_of<HierarchyComponent>(entity))
            {
                auto& hierarchy = registry.get<HierarchyComponent>(entity);
                item.Parent = hierarchy.m_Parent;
                item.Next = hierarchy.m_Next;
            }

            s_SortedUIItems.push_back(item);
        }

        std::sort(s_SortedUIItems.begin(), s_SortedUIItems.end(), [&registry](const UIRenderItem& a, const UIRenderItem& b) {
            if (a.Layer != b.Layer)
                return a.Layer < b.Layer;

            if (a.Parent == b.Parent)
            {
                entt::entity current = a.Entity;
                while (current != entt::null)
                {
                    if (current == b.Entity)
                        return true;

                    if (registry.any_of<HierarchyComponent>(current))
                        current = registry.get<HierarchyComponent>(current).m_Next;
                }

                return false;
            }

            {
                entt::entity parent = b.Parent;
                while (parent != entt::null)
                {
                    if (parent == a.Entity)
                        return true;

                    if (registry.any_of<HierarchyComponent>(parent))
                        parent = registry.get<HierarchyComponent>(parent).m_Parent;
                }
            }

            {
                entt::entity parent = a.Parent;
                while (parent != entt::null)
                {
                    if (parent == b.Entity)
                        return false;

                    if (registry.any_of<HierarchyComponent>(parent))
                        parent = registry.get<HierarchyComponent>(parent).m_Parent;
                }
            }

            return static_cast<uint32_t>(a.Entity) < static_cast<uint32_t>(b.Entity);
        });
    }

    void UIManager::RenderUIImage(entt::registry& registry, entt::entity entity)
    {
        auto& uiImageComponent = registry.get<UIImageComponent>(entity);
        auto& transformComponent = registry.get<TransformComponent>(entity);
        auto& anchor = uiImageComponent.Anchor;

        glm::vec2 anchoredPosition;
        glm::vec2 anchoredSize;

        anchor.CalculateTransformData(glm::vec2(WindowSize), anchoredPosition, anchoredSize);

        transformComponent.SetLocalPosition(glm::vec3(anchoredPosition, 0.0f));
        transformComponent.SetLocalScale(glm::vec3(anchoredSize.x, anchoredSize.y, 1.0f));

        float rotation = transformComponent.GetLocalRotation().z;

        glm::mat4 worldTransform = glm::mat4(1.0f);
        worldTransform = glm::translate(worldTransform, glm::vec3(anchoredPosition, 0.0f));
        worldTransform = glm::rotate(worldTransform, glm::radians(rotation), glm::vec3(0.0f, 0.0f, 1.0f));
        worldTransform = glm::scale(worldTransform, glm::vec3(anchoredSize.x, anchoredSize.y, 1.0f));

        Renderer2D::DrawQuad(worldTransform, uiImageComponent.Texture, 1.0f, glm::vec4(1.0f), Renderer2D::RenderMode::Screen, (uint32_t)entity);
    }

    void UIManager::RenderUIText(entt::registry& registry, entt::entity entity)
    {
        auto& uiTextComponent = registry.get<UITextComponent>(entity);
        auto& transformComponent = registry.get<TransformComponent>(entity);

        if (!uiTextComponent.Font)
            uiTextComponent.Font = Font::GetDefault();

        glm::vec2 anchoredPosition;
        glm::vec2 anchoredSize;

        uiTextComponent.Anchor.CalculateTransformData(glm::vec2(WindowSize), anchoredPosition, anchoredSize);

        float rotation = transformComponent.GetLocalRotation().z;

        glm::mat4 textTransform = glm::mat4(1.0f);
        textTransform = glm::translate(textTransform, glm::vec3(anchoredPosition, 0.0f));
        textTransform = glm::rotate(textTransform, glm::radians(rotation), glm::vec3(0.0f, 0.0f, 1.0f));

        Renderer2D::TextParams textParams;
        textParams.Color = uiTextComponent.Color;
        textParams.Kerning = uiTextComponent.Kerning;
        textParams.LineSpacing = uiTextComponent.LineSpacing;
        textParams.Size = uiTextComponent.FontSize;

        Renderer2D::DrawTextString(uiTextComponent.Text, uiTextComponent.Font, textTransform, textParams, Renderer2D::RenderMode::Screen, (uint32_t)entity);

        transformComponent.SetLocalPosition(glm::vec3(anchoredPosition, 0.0f));
        transformComponent.SetLocalScale(glm::vec3(1.0f));
    }

    void UIManager::RenderUIToggle(entt::registry& registry, entt::entity entity)
    {
        auto& toggleComponent = registry.get<UIToggleComponent>(entity);
        auto& transformComponent = registry.get<TransformComponent>(entity);

        glm::vec2 anchoredPosition;
        glm::vec2 anchoredSize;
        toggleComponent.Anchor.CalculateTransformData(glm::vec2(WindowSize), anchoredPosition, anchoredSize);

        transformComponent.SetLocalPosition(glm::vec3(anchoredPosition, 0.0f));
        transformComponent.SetLocalScale(glm::vec3(anchoredSize.x, anchoredSize.y, 1.0f));

        float rotation = transformComponent.GetLocalRotation().z;
        glm::mat4 worldTransform = glm::mat4(1.0f);
        worldTransform = glm::translate(worldTransform, glm::vec3(anchoredPosition, 0.0f));
        worldTransform = glm::rotate(worldTransform, glm::radians(rotation), glm::vec3(0.0f, 0.0f, 1.0f));
        worldTransform = glm::scale(worldTransform, glm::vec3(anchoredSize.x, anchoredSize.y, 1.0f));

        Ref<Texture2D> currentTexture = toggleComponent.Value ? toggleComponent.OnTexture : toggleComponent.OffTexture;

        Renderer2D::DrawQuad(worldTransform, currentTexture, 1.0f, glm::vec4(1.0f), Renderer2D::RenderMode::Screen, (uint32_t)entity);
    }

    void UIManager::RenderUIButton(entt::registry& registry, entt::entity entity)
    {
        auto& button = registry.get<UIButtonComponent>(entity);
        auto& transform = registry.get<TransformComponent>(entity);

        glm::vec2 anchoredPosition;
        glm::vec2 anchoredSize;
        button.Anchor.CalculateTransformData(glm::vec2(WindowSize), anchoredPosition, anchoredSize);

        transform.SetLocalPosition(glm::vec3(anchoredPosition, 0.0f));
        transform.SetLocalScale(glm::vec3(anchoredSize.x, anchoredSize.y, 1.0f));

        float rotation = transform.GetLocalRotation().z;
        glm::mat4 worldTransform = glm::mat4(1.0f);
        worldTransform = glm::translate(worldTransform, glm::vec3(anchoredPosition, 0.0f));
        worldTransform = glm::rotate(worldTransform, glm::radians(rotation), glm::vec3(0.0f, 0.0f, 1.0f));
        worldTransform = glm::scale(worldTransform, glm::vec3(anchoredSize.x, anchoredSize.y, 1.0f));

        Ref<Texture2D> currentTexture = nullptr;
        glm::vec4 currentColor{1.0f};

        switch (button.CurrentState)
        {
            case UIButtonComponent::State::Normal:
                currentTexture = button.NormalTexture;
                currentColor = button.NormalColor;
                break;
            case UIButtonComponent::State::Hover:
                currentTexture = button.HoverTexture;
                currentColor = button.HoverColor;
                break;
            case UIButtonComponent::State::Pressed:
                currentTexture = button.PressedTexture;
                currentColor = button.PressedColor;
                break;
            case UIButtonComponent::State::Disabled:
                currentTexture = button.DisabledTexture;
                currentColor = button.DisabledColor;
                break;
        }

        if (currentTexture)
            Renderer2D::DrawQuad(worldTransform, currentTexture, 1.0f, currentColor, Renderer2D::RenderMode::Screen, (uint32_t)entity);
    }

    AnchorPreset UIManager::GetAnchorPreset(int row, int column)
    {
        // Row: 0=top, 1=middle, 2=bottom, 3=stretch
        // Column: 0=left, 1=center, 2=right, 3=stretch

        AnchorPresetY y;
        switch(row)
        {
            case 0: y = AnchorPresetY::Top; break;
            case 1: y = AnchorPresetY::Middle; break;
            case 2: y = AnchorPresetY::Bottom; break;
            case 3: y = AnchorPresetY::Stretch; break;
            default: y = AnchorPresetY::Top;
        }

        AnchorPresetX x;
        switch(column)
        {
            case 0: x = AnchorPresetX::Left; break;
            case 1: x = AnchorPresetX::Center; break;
            case 2: x = AnchorPresetX::Right; break;
            case 3: x = AnchorPresetX::Stretch; break;
            default: x = AnchorPresetX::Left;
        }

        return AnchorPreset(x, y);
    }

} // Coffee