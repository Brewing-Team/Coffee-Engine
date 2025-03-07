#include "UIManager.h"
#include "../IO/ResourceLoader.h"
#include "../Renderer/Renderer.h"
#include "../Renderer/Renderer2D.h"

namespace Coffee {

    void UIManager::OnEditorUpdate(float dt, entt::registry& registry) {
        // Get the window size and calculate the center
        auto windowSize = Renderer::GetCurrentRenderTarget()->GetSize();
        glm::vec2 center = glm::vec2(windowSize.x / 2.0f, windowSize.y / 2.0f);

        // Render UI elements (UIImageComponent)
        auto uiImageView = registry.view<UIImageComponent, TransformComponent>();
        for (auto& entity : uiImageView) {
            auto& uiImageComponent = uiImageView.get<UIImageComponent>(entity);
            auto& transformComponent = uiImageView.get<TransformComponent>(entity);

            if (!uiImageComponent.Visible || uiImageComponent.TexturePath.empty())
                continue;

            // Load the image texture if not already loaded
            if (!uiImageComponent.Texture) {
                uiImageComponent.Texture = ResourceLoader::LoadTexture2D(uiImageComponent.TexturePath, true, true);

                if (!uiImageComponent.Texture) {
                    COFFEE_ERROR("Failed to load texture: {}", uiImageComponent.TexturePath);
                    continue;
                }
            }

            // Calculate the transform to center the image
            glm::mat4 transform = transformComponent.GetWorldTransform();
            transform = glm::translate(transform, glm::vec3(center, 0.0f)); // Center the image
            transform = glm::scale(transform, glm::vec3(uiImageComponent.Size.x, uiImageComponent.Size.y, 1.0f));

            // Render the image
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

            // Use the default font if no font is set
            if (!uiTextComponent.font) {
                uiTextComponent.font = Font::GetDefault();
            }

            // Calculate the transform to center the text
            glm::mat4 transform = transformComponent.GetWorldTransform();
            transform = glm::translate(transform, glm::vec3(center, 0.0f)); // Center the text
            transform = glm::scale(transform, glm::vec3(uiTextComponent.FontSize, -uiTextComponent.FontSize, 1.0f));

            // Render the text
            Renderer2D::DrawText(
                uiTextComponent.Text,
                uiTextComponent.font,
                transform,
                {uiTextComponent.Color, 0.0f, 0.0f}, // Text color
                (uint32_t)entity
            );
        }
    }


    void UIManager::OnRuntimeUpdate(float dt, entt::registry& registry) {
        // Get the window size and calculate the center
        auto windowSize = Renderer::GetCurrentRenderTarget()->GetSize();
        glm::vec2 center = glm::vec2(windowSize.x / 2.0f, windowSize.y / 2.0f);

        // Render UI elements (UIImageComponent)
        auto uiImageView = registry.view<UIImageComponent, TransformComponent>();
        for (auto& entity : uiImageView) {
            auto& uiImageComponent = uiImageView.get<UIImageComponent>(entity);
            auto& transformComponent = uiImageView.get<TransformComponent>(entity);

            if (!uiImageComponent.Visible || uiImageComponent.TexturePath.empty())
                continue;

            // Load the image texture if not already loaded
            if (!uiImageComponent.Texture) {
                uiImageComponent.Texture = ResourceLoader::LoadTexture2D(uiImageComponent.TexturePath, true, true);

                if (!uiImageComponent.Texture) {
                    COFFEE_ERROR("Failed to load texture: {}", uiImageComponent.TexturePath);
                    continue;
                }
            }

            // Calculate the transform to center the image
            glm::mat4 transform = transformComponent.GetWorldTransform();
            transform = glm::translate(transform, glm::vec3(center, 0.0f)); // Center the image
            transform = glm::scale(transform, glm::vec3(uiImageComponent.Size.x, uiImageComponent.Size.y, 1.0f));

            // Render the image
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

            // Use the default font if no font is set
            if (!uiTextComponent.font) {
                uiTextComponent.font = Font::GetDefault();
            }

            // Calculate the transform to center the text
            glm::mat4 transform = transformComponent.GetWorldTransform();
            transform = glm::translate(transform, glm::vec3(center, 0.0f)); // Center the text
            transform = glm::scale(transform, glm::vec3(uiTextComponent.FontSize, -uiTextComponent.FontSize, 1.0f));

            // Render the text
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