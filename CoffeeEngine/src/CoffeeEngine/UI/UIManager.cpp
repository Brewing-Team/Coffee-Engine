#include "UIManager.h"
#include "../IO/ResourceLoader.h"
#include "../Renderer/Renderer.h"
#include "../Renderer/Renderer2D.h"
#include "../IO/ResourceRegistry.h"

namespace Coffee {

    void UIManager::OnEditorUpdate(float dt, entt::registry& registry) {
        auto windowSize = Renderer::GetCurrentRenderTarget()->GetSize();
        glm::vec2 center = glm::vec2(windowSize.x / 2.0f, windowSize.y / 2.0f);

        auto uiImageView = registry.view<UIImageComponent, TransformComponent>();
        for (auto& entity : uiImageView) {
            auto& uiImageComponent = uiImageView.get<UIImageComponent>(entity);
            auto& transformComponent = uiImageView.get<TransformComponent>(entity);

            if (!uiImageComponent.Visible || !uiImageComponent.material)
                continue;

            Ref<Texture2D> texture = uiImageComponent.material->GetMaterialTextures().albedo;
            if (!texture)
                continue;

            glm::mat4 transform = transformComponent.GetWorldTransform();
            transform = glm::translate(transform, glm::vec3(center, 0.0f));
            transform = glm::scale(transform, glm::vec3(uiImageComponent.Size.x, uiImageComponent.Size.y, 1.0f));

            Renderer2D::DrawQuad(
                transform,
                texture,
                1.0f,
                glm::vec4(1.0f),
                (uint32_t)entity
            );
        }

        auto uiTextView = registry.view<UITextComponent, TransformComponent>();
        for (auto& entity : uiTextView) {
            auto& uiTextComponent = uiTextView.get<UITextComponent>(entity);
            auto& transformComponent = uiTextView.get<TransformComponent>(entity);

            if (!uiTextComponent.Visible || uiTextComponent.Text.empty())
                continue;

            if (!uiTextComponent.font) {
                uiTextComponent.font = Font::GetDefault();
            }

            glm::mat4 transform = transformComponent.GetWorldTransform();
            transform = glm::translate(transform, glm::vec3(center, 0.0f));
            transform = glm::scale(transform, glm::vec3(uiTextComponent.FontSize, -uiTextComponent.FontSize, 1.0f));

            Renderer2D::DrawText(
                uiTextComponent.Text,
                uiTextComponent.font,
                transform,
                {uiTextComponent.Color, 0.0f, 0.0f},
                (uint32_t)entity
            );
        }
    }

    void UIManager::OnRuntimeUpdate(float dt, entt::registry& registry) {
        auto windowSize = Renderer::GetCurrentRenderTarget()->GetSize();
        glm::vec2 center = glm::vec2(windowSize.x / 2.0f, windowSize.y / 2.0f);

        auto uiImageView = registry.view<UIImageComponent, TransformComponent>();
        for (auto& entity : uiImageView) {
            auto& uiImageComponent = uiImageView.get<UIImageComponent>(entity);
            auto& transformComponent = uiImageView.get<TransformComponent>(entity);

            if (!uiImageComponent.Visible || !uiImageComponent.material)
                continue;

            Ref<Texture2D> texture = uiImageComponent.material->GetMaterialTextures().albedo;
            if (!texture)
                continue;

            glm::mat4 transform = transformComponent.GetWorldTransform();
            transform = glm::translate(transform, glm::vec3(center, 0.0f));
            transform = glm::scale(transform, glm::vec3(uiImageComponent.Size.x, uiImageComponent.Size.y, 1.0f));

            Renderer2D::DrawQuad(
                transform,
                texture,
                1.0f,
                glm::vec4(1.0f),
                (uint32_t)entity
            );
        }

        auto uiTextView = registry.view<UITextComponent, TransformComponent>();
        for (auto& entity : uiTextView) {
            auto& uiTextComponent = uiTextView.get<UITextComponent>(entity);
            auto& transformComponent = uiTextView.get<TransformComponent>(entity);

            if (!uiTextComponent.Visible || uiTextComponent.Text.empty())
                continue;

            if (!uiTextComponent.font) {
                uiTextComponent.font = Font::GetDefault();
            }

            glm::mat4 transform = transformComponent.GetWorldTransform();
            transform = glm::translate(transform, glm::vec3(center, 0.0f));
            transform = glm::scale(transform, glm::vec3(uiTextComponent.FontSize, -uiTextComponent.FontSize, 1.0f));

            Renderer2D::DrawText(
                uiTextComponent.Text,
                uiTextComponent.font,
                transform,
                {uiTextComponent.Color, 0.0f, 0.0f},
                (uint32_t)entity
            );
        }
    }
}