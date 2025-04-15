#include "UIManager.h"

#include "CoffeeEngine/Renderer/Renderer.h"
#include "CoffeeEngine/Renderer/Renderer2D.h"
#include "CoffeeEngine/Scene/Components.h"

namespace Coffee {

    void UIManager::UpdateUI(entt::registry& registry)
    {
        auto windowSize = Renderer::GetCurrentRenderTarget()->GetSize();
        auto uiImageView = registry.view<UIImageComponent, TransformComponent>();

        for (auto entity : uiImageView)
        {
            auto& uiImageComponent = uiImageView.get<UIImageComponent>(entity);
            auto& transformComponent = uiImageView.get<TransformComponent>(entity);

            glm::vec2 anchorOffset = glm::vec2(windowSize.x / 2.0f, windowSize.y / 2.0f);
            glm::mat4 worldTransform = glm::mat4(1.0f);
            glm::vec2 finalPosition = anchorOffset + glm::vec2(transformComponent.GetLocalPosition());
            worldTransform = glm::translate(worldTransform, glm::vec3(finalPosition, 0.0f));
            worldTransform = glm::rotate(worldTransform, glm::radians(transformComponent.GetLocalRotation().z), glm::vec3(0.0f, 0.0f, 1.0f));
            worldTransform = glm::scale(worldTransform, glm::vec3(transformComponent.GetLocalScale().x, transformComponent.GetLocalScale().y, 1.0f));

            Renderer2D::DrawQuad(worldTransform, uiImageComponent.Texture, 1.0f, glm::vec4(1.0f), Renderer2D::RenderMode::Screen, (uint32_t)entity);
        }
    }

} // Coffee