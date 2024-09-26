#include "Entity.h"
#include "CoffeeEngine/Scene/Scene.h"
#include "entt/entity/fwd.hpp"

namespace Coffee {

    Entity::Entity(entt::entity handle, Scene* scene)
        : m_EntityHandle(handle), m_Scene(scene)
    {
    }

}