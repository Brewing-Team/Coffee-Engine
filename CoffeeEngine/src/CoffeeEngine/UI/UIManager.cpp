#include "UIManager.h"

#include "CoffeeEngine/Renderer/Renderer.h"
#include "CoffeeEngine/Renderer/Renderer2D.h"
#include "CoffeeEngine/Scene/Components.h"

namespace Coffee {

    glm::vec2 UIManager::WindowSize;

    void UIManager::UpdateUI(entt::registry& registry)
    {
        WindowSize = Renderer::GetCurrentRenderTarget()->GetSize();
        auto uiImageView = registry.view<UIImageComponent, TransformComponent>();

        for (auto entity : uiImageView)
        {
            auto& uiImageComponent = uiImageView.get<UIImageComponent>(entity);
            auto& transformComponent = uiImageView.get<TransformComponent>(entity);
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

        auto uiTextView = registry.view<UITextComponent, TransformComponent>();

        for (auto entity : uiTextView)
        {
            auto& uiTextComponent = uiTextView.get<UITextComponent>(entity);
            auto& transformComponent = uiTextView.get<TransformComponent>(entity);

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