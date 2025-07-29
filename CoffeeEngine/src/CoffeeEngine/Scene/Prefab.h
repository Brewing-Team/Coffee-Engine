#pragma once

#include "CoffeeEngine/Core/Base.h"
#include "CoffeeEngine/Core/UUID.h"
#include "CoffeeEngine/IO/Resource.h"
#include "CoffeeEngine/Scene/Entity.h"

#include <entt/entt.hpp>
#include <filesystem>
#include <unordered_map>

namespace Coffee {

    class Prefab : public Resource {
    public:
        Prefab();
        ~Prefab() = default;
        
        static Ref<Prefab> Create(Entity entity);
        
        Entity Instantiate(Scene* scene, const glm::mat4& transform = glm::mat4(1.0f));
        
        bool Save(const std::filesystem::path& path);
        static Ref<Prefab> Load(const std::filesystem::path& path);
        
    private:
        // Helper templates to simplify component copying
        template<typename T>
        void CopyComponentToPrefab(Entity source, entt::entity dest) {
            if (source.HasComponent<T>())
                m_Registry.emplace<T>(dest, source.GetComponent<T>());
        }
        
        template<typename T>
        void CopyEmptyComponentToPrefab(Entity source, entt::entity dest) {
            if (source.HasComponent<T>())
                m_Registry.emplace<T>(dest);
        }
        
        template<typename T>
        void CopyComponentToScene(Scene* scene, entt::entity source, Entity& dest) {
            if (m_Registry.all_of<T>(source))
                dest.AddComponent<T>(m_Registry.get<T>(source));
        }
        
        template<typename T>
        void CopyEmptyComponentToScene(Scene* scene, entt::entity source, Entity& dest) {
            if (m_Registry.all_of<T>(source))
                dest.AddComponent<T>();
        }
        
        entt::entity CopyEntityToPrefab(Entity sourceEntity, entt::entity parentEntity = entt::null);
        Entity CopyEntityToScene(Scene* scene, entt::entity prefabEntity, Entity parent);
        
        // Cereal serialization
        friend class cereal::access;
        
        template<class Archive>
        void save(Archive& archive, std::uint32_t const version) const;
        
        template<class Archive>
        void load(Archive& archive, std::uint32_t const version);
        
    private:
        entt::registry m_Registry;
        entt::entity m_RootEntity = entt::null;
        std::unordered_map<UUID, UUID> m_EntityMap;
    };

} // namespace Coffee

CEREAL_CLASS_VERSION(Coffee::Prefab, 1);