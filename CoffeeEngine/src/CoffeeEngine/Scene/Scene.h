#pragma once

#include "CoffeeEngine/Core/DataStructures/Octree.h"
#include "CoffeeEngine/Physics/PhysicsWorld.h" // Think removing it using Scope<PhysicsWorld> instead

#include <cereal/cereal.hpp>

#include <entt/entt.hpp>
#include <filesystem>

namespace Coffee {

    class EditorCamera;
    class Event;
    class SceneTree;
    class UUID;
}

namespace Coffee {

    /**
     * @defgroup scene Scene
     * @{
     */

    struct AnimatorComponent;
    struct MeshComponent;
    class Entity;
    class Model;

    struct SceneDebugFlags
    {
        bool DebugDraw = false;
        bool ShowNavMesh = false;
        bool ShowNavMeshPath = false;
        bool ShowColliders = false;
        bool ShowOctree = false;
    };

    /**
     * @brief Class representing a scene.
     * @ingroup scene
     */
    class Scene
    {
    public:

        /**
         * @brief Constructor for Scene.
         */
        Scene();

        /**
         * @brief Default destructor.
         */
        ~Scene() = default;

        //Scene(Ref<Scene> other);

        /**
         * @brief Create an entity in the scene.
         * @param name The name of the entity.
         * @return The created entity.
         */
        Entity CreateEntity(const std::string& name = std::string());

        Entity DuplicateEntityRecursive(Entity& sourceEntity, Entity* parentEntity);

        Entity Duplicate(Entity& parent);

        /**
         * @brief Destroy an entity in the scene.
         * @param entity The entity to destroy.
         */
        void DestroyEntity(Entity entity);

        Entity GetEntityByName(const std::string& name);

        std::vector<Entity> GetAllEntities();

        template<typename... Components>
        auto GetAllEntitiesWithComponents()
        {
            return m_Registry.view<Components...>();
        }

        /**
         * @brief Initialize the scene.
         */
        void OnInitEditor();
        void OnInitRuntime();

        /**
         * @brief Update the scene in editor mode.
         * @param camera The editor camera.
         * @param dt The delta time.
         */
        void OnUpdateEditor(EditorCamera& camera, float dt);

        /**
         * @brief Update the scene in runtime mode.
         * @param dt The delta time.
         */
        void OnUpdateRuntime(float dt);

        /**
         * @brief Handle an event in the scene.
         * @param e The event.
         */
        void OnEvent(Event& e);

        /**
         * @brief Exit the scene.
         */
        void OnExitEditor();
        void OnExitRuntime();

        const PhysicsWorld& GetPhysicsWorld() const { return m_PhysicsWorld; }
        PhysicsWorld& GetPhysicsWorld() { return m_PhysicsWorld; }

        /**
         * @brief Load a scene from a file.
         * @param path The path to the file.
         * @return The loaded scene.
         */
        static Ref<Scene> Load(const std::filesystem::path& path);

        /**
         * @brief Save a scene to a file.
         * @param path The path to the file.
         * @param scene The scene to save.
         */
        static void Save(const std::filesystem::path& path, Ref<Scene> scene);

        const std::filesystem::path& GetFilePath() const { return m_FilePath; }
        void SetFilePath(const std::filesystem::path& path) { m_FilePath = path; }

        bool IsLoading() const { return m_IsLoading; }

        /**
         * @brief Update the positions of the audio components.
         */
        void UpdateAudioComponentsPositions();

        const std::filesystem::path& GetFilePath() { return m_FilePath; }

        SceneDebugFlags& GetDebugFlags() { return m_SceneDebugFlags; }



        /**
         * @brief Assigns animators to meshes.
         * @param animators The vector of animator components.
         */
        void AssignAnimatorsToMeshes(const std::vector<AnimatorComponent*> animators);

        static std::map<UUID, UUID> s_UUIDMap;
        static std::vector<MeshComponent*> s_MeshComponents;
        static std::vector<AnimatorComponent*> s_AnimatorComponents;

    private:
        friend class cereal::access;

        /**
         * @brief Serializes the scene to an archive.
         * @tparam Archive The type of the archive.
         * @param archive The archive to save the scene to.
         */
         template <class Archive> void save(Archive& archive, std::uint32_t const version) const;

        /**
         * @brief Deserializes the scene from an archive.
         * @tparam Archive The type of the archive.
         * @param archive The archive to load the scene from.
         */
        template <class Archive> void load(Archive& archive, std::uint32_t const version);

    private:
        // NOTE: this macro should be modified when adding new components
        #define ALL_COMPONENTS \
            TagComponent, TransformComponent, HierarchyComponent, CameraComponent, \
            MeshComponent, MaterialComponent, LightComponent, RigidbodyComponent, \
            ScriptComponent, AudioSourceComponent, AudioListenerComponent, AudioZoneComponent, \
            AnimatorComponent, ActiveComponent, StaticComponent, \
            UIComponent, UIImageComponent, UITextComponent, UIToggleComponent, UIButtonComponent, UISliderComponent, \
            WorldEnvironmentComponent

        entt::registry m_Registry;
        Scope<SceneTree> m_SceneTree;
        Scope<Octree<entt::entity>> m_Octree;
        PhysicsWorld m_PhysicsWorld;
        SceneDebugFlags m_SceneDebugFlags;

        // Temporal: Scenes should be Resources and the Base Resource class already has a path variable.
        std::filesystem::path m_FilePath;

        bool m_IsLoading = false;

        friend class Entity;
        friend class SceneTree;
        friend class SceneTreePanel;
        friend class CollisionSystem;
    };

    /**
     * @brief Add a model to the scene tree.
     * @param scene The scene.
     * @param model The model to add.
     */
    void AddModelToTheSceneTree(Scene* scene, Ref<Model> model, AnimatorComponent* animatorComponent = nullptr);

    /** @} */ // end of scene group
} // namespace Coffee
CEREAL_CLASS_VERSION(Coffee::Scene, 3);
