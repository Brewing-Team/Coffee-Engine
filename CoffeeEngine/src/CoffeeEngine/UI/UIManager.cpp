#include "UIManager.h"
#include "../IO/ResourceLoader.h"
#include "../Renderer/Renderer.h"
#include "../Renderer/Renderer2D.h"

namespace Coffee {

    void UIManager::OnEditorUpdate(float dt, entt::registry& registry) {
    // Render UI elements (UICanvasComponent)
    auto uiCanvasView = registry.view<UICanvasComponent, TransformComponent>();
    for (auto& entity : uiCanvasView) {
        auto& uiCanvasComponent = uiCanvasView.get<UICanvasComponent>(entity);
        auto& transformComponent = uiCanvasView.get<TransformComponent>(entity);

        if (!uiCanvasComponent.Visible)
            continue;

        if (!uiCanvasComponent.CanvasTexture) {
            uiCanvasComponent.CanvasTexture = ResourceLoader::LoadTexture2D("assets/textures/Canvas.png", true, true);

            if (!uiCanvasComponent.CanvasTexture) {
                COFFEE_ERROR("Failed to load canvas texture: assets/textures/Canvas.png");
                continue;
            }
        }

        auto windowSize = Renderer::GetCurrentRenderTarget()->GetSize();
        glm::vec2 center = glm::vec2(windowSize.x / 2.0f, windowSize.y / 2.0f);

        glm::mat4 transform = glm::mat4(1.0f);
        transform = glm::translate(transform, glm::vec3(center, 0.0f));
        transform = glm::scale(transform, glm::vec3(windowSize.x, windowSize.y, 1.0f));

        Renderer2D::DrawQuad(
            transform,
            uiCanvasComponent.CanvasTexture,
            1.0f,
            glm::vec4(1.0f),
            (uint32_t)entity
        );
    }

    // Render UI elements (UIImageComponent)
    auto uiImageView = registry.view<UIImageComponent, TransformComponent>();
    for (auto& entity : uiImageView) {
        auto& uiImageComponent = uiImageView.get<UIImageComponent>(entity);
        auto& transformComponent = uiImageView.get<TransformComponent>(entity);

        if (!uiImageComponent.Visible || uiImageComponent.TexturePath.empty())
            continue;

        if (!uiImageComponent.Texture) {
            uiImageComponent.Texture = ResourceLoader::LoadTexture2D(uiImageComponent.TexturePath, true, true);

            if (!uiImageComponent.Texture) {
                COFFEE_ERROR("Failed to load texture: {}", uiImageComponent.TexturePath);
                continue;
            }
        }

        glm::mat4 transform = transformComponent.GetWorldTransform();
        transform = glm::scale(transform, glm::vec3(uiImageComponent.Size.x, uiImageComponent.Size.y, 1.0f));

        Renderer2D::DrawQuad(
            transform,
            uiImageComponent.Texture,
            1.0f,
            glm::vec4(1.0f),
            (uint32_t)entity
        );
    }

    // Render UI elements (UITextComponent)
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
        auto uiImageView = registry.view<UIImageComponent, TransformComponent>();
        for (auto& entity : uiImageView)
        {
            auto& uiImageComponent = uiImageView.get<UIImageComponent>(entity);
            auto& transformComponent = uiImageView.get<TransformComponent>(entity);

            if (!uiImageComponent.Visible || uiImageComponent.TexturePath.empty())
                continue;

            if (!uiImageComponent.Texture)
            {
                uiImageComponent.Texture = ResourceLoader::LoadTexture2D(uiImageComponent.TexturePath, true, true);
                if (!uiImageComponent.Texture)
                {
                    COFFEE_ERROR("Failed to load texture: {}", uiImageComponent.TexturePath);
                    continue;
                }
            }

            COFFEE_INFO("Rendering UIImageComponent - Entity: {}", (uint32_t)entity);

            glm::mat4 transform = transformComponent.GetWorldTransform();
            transform = glm::scale(transform, glm::vec3(uiImageComponent.Size.x, uiImageComponent.Size.y, 1.0f));

            Renderer2D::DrawQuad(
                transform,
                uiImageComponent.Texture,
                1.0f,
                glm::vec4(1.0f),
                (uint32_t)entity
            );
        }

        // Renderizar UI elements (UITextComponent)
        auto uiTextView = registry.view<UITextComponent, TransformComponent>();
        for (auto& entity : uiTextView)
        {
            auto& uiTextComponent = uiTextView.get<UITextComponent>(entity);
            auto& transformComponent = uiTextView.get<TransformComponent>(entity);

            if (!uiTextComponent.Visible || uiTextComponent.Text.empty())
                continue;

            if (!uiTextComponent.font)
            {
                uiTextComponent.font = Font::GetDefault();
            }

            COFFEE_INFO("Rendering UITextComponent - Entity: {}", (uint32_t)entity);

            glm::mat4 transform = transformComponent.GetWorldTransform();
            transform = glm::scale(transform, glm::vec3(uiTextComponent.FontSize, -uiTextComponent.FontSize, 1.0f));

            Renderer2D::DrawText(
                uiTextComponent.Text,
                uiTextComponent.font,
                transform,
                {uiTextComponent.Color, 0.0f, 0.0f}, // Text color
                (uint32_t)entity
            );
        }

    }

}