#include "Entity.h"
#include "TeaEngine/Scene/Scene.h"
#include "entt/entity/fwd.hpp"

namespace Tea {

    Entity::Entity(entt::entity handle, Scene* scene)
        : m_EntityHandle(handle), m_Scene(scene)
    {
    }

}