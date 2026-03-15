#include "ResourceResolver.h"

#include "CoffeeEngine/Core/Log.h"
#include "CoffeeEngine/Scene/Components/UIButtonComponent.h"
#include "CoffeeEngine/Scene/Components/UIImageComponent.h"
#include "CoffeeEngine/Scene/Components/UISliderComponent.h"
#include "CoffeeEngine/Scene/Components/UITextComponent.h"
#include "CoffeeEngine/Scene/Components/UIToggleComponent.h"
#include "CoffeeEngine/Scene/Components/MaterialComponent.h"
#include "CoffeeEngine/Scene/Components/MeshComponent.h"
#include "CoffeeEngine/Scene/Components/ScriptComponent.h"
#include "CoffeeEngine/Scene/Components/WorldEnvironmentComponent.h"
#include "CoffeeEngine/Scene/Scene.h"

namespace Coffee {

    namespace
    {
        template <typename Component>
        void ResolveComponentView(entt::registry& registry, EngineContext& context)
        {
            auto view = registry.view<Component>();
            for (auto entity : view)
                view.template get<Component>(entity).ResolveResources(context);
        }
    }

    void ResourceResolver::ResolveSceneResourceReferences(Scene& scene) const
    {
        if (!m_ResourceManager)
        {
            COFFEE_CORE_WARN("ResourceResolver::ResolveSceneResourceReferences: ResourceManager is null.");
            return;
        }

        ResolveComponentView<MaterialComponent>(scene.m_Registry, scene.m_Context);
        ResolveComponentView<UIImageComponent>(scene.m_Registry, scene.m_Context);
        ResolveComponentView<UIButtonComponent>(scene.m_Registry, scene.m_Context);
        ResolveComponentView<UIToggleComponent>(scene.m_Registry, scene.m_Context);
        ResolveComponentView<UISliderComponent>(scene.m_Registry, scene.m_Context);
        ResolveComponentView<MeshComponent>(scene.m_Registry, scene.m_Context);
        ResolveComponentView<WorldEnvironmentComponent>(scene.m_Registry, scene.m_Context);
        ResolveComponentView<ScriptComponent>(scene.m_Registry, scene.m_Context);
        ResolveComponentView<UITextComponent>(scene.m_Registry, scene.m_Context);
    }

}