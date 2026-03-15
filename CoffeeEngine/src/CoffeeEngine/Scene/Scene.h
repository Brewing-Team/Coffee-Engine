#pragma once

#include "CoffeeEngine/Core/EngineContext.h"
#include "CoffeeEngine/Core/DataStructures/Octree.h"
#include "CoffeeEngine/Physics/Runtime/PhysicsWorld.h" // Think removing it using Scope<PhysicsWorld> instead
#include "CoffeeEngine/Scene/Systems/AnimationSystem.h"
#include "CoffeeEngine/Scene/Systems/CollisionSystem.h"
#include "CoffeeEngine/Scene/Systems/UISystem.h"

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

    class EngineContext;
    class Audio;
    class ScriptingManager;

    struct AnimatorComponent;
    struct MeshComponent;
    class Entity;
    class Model;

    /**
     * @defgroup scene Scene
     * @{
     */

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
        Scene(EngineContext& context);;

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
        void OnExit();
        void OnExitEditor();
        void OnExitRuntime();

        const PhysicsWorld& GetPhysicsWorld() const { return m_PhysicsWorld; }
        PhysicsWorld& GetPhysicsWorld() { return m_PhysicsWorld; }

        /**
         * @brief Load a scene from a file.
         * @param path The path to the file.
         * @return The loaded scene.
         */
        // TODO: Move this out of the Scene class. It should be managed by the ResourceManager when the Scene class becomes a Resource.
        static Ref<Scene> Load(const std::filesystem::path& path, EngineContext& context);

        /**
         * @brief Save a scene to a file.
         * @param path The path to the file.
         * @param scene The scene to save.
         */
        // TODO: Move this out of the Scene class. It should be managed by the ResourceManager when the Scene class becomes a Resource.
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

        ScriptingManager* GetScriptingManager() const { return m_Context.scripting; }
        Audio* GetAudio() const { return m_Context.audio; }
        Renderer* GetRenderer() const { return m_Context.renderer; }
        ResourceManager* GetResourceManager() const { return m_Context.resourceManager; }



        /**
         * @brief Assigns animators to meshes.
         * @param animators The vector of animator components.
         */
        void AssignAnimatorsToMeshes(const std::vector<AnimatorComponent*> animators);

        void LoadAnimator(AnimatorComponent* animator) { m_AnimationSystem.LoadAnimator(animator); }
        void SetupPartialBlending(unsigned int upper, unsigned int lower, const std::string& joint, AnimatorComponent* animator) { m_AnimationSystem.SetupPartialBlending(upper, lower, joint, animator); }
        std::vector<AnimatorComponent*> GetAnimators() { return m_AnimationSystem.GetAnimators(); }
        UISystem& GetUISystem() { return m_UISystem; }

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
        // INFO: I'm still learning how to use the new architecture, but I think that the Scene should not have a reference to the EngineContext. The Scene should be a Resource that is managed by the ResourceManager and should not have direct access to the EngineContext. However, for now, I'm adding it here to avoid having to refactor a lot of code that relies on the Scene having access to the EngineContext. In the future, when we refactor the code, we should remove this reference and find a better way to access the EngineContext from the Scene when needed.
        EngineContext& m_Context;

        entt::registry m_Registry;
        Scope<SceneTree> m_SceneTree;
        Scope<Octree<entt::entity>> m_Octree;
        PhysicsWorld m_PhysicsWorld;
        SceneDebugFlags m_SceneDebugFlags;

        // Systems
        AnimationSystem m_AnimationSystem;
        CollisionSystem m_CollisionSystem;
        UISystem m_UISystem;

        // Temporal: Scenes should be Resources and the Base Resource class already has a path variable.
        std::filesystem::path m_FilePath;

        bool m_IsLoading = false;

        friend class Entity;
        friend class SceneTree;
        friend class SceneTreePanel;
        friend class CollisionSystem;
        friend class ResourceResolver;

        // NOTE: this macro should be modified when adding new components
        #define ALL_COMPONENTS \
            TagComponent, TransformComponent, HierarchyComponent, CameraComponent, \
            MeshComponent, MaterialComponent, LightComponent, RigidbodyComponent, \
            ScriptComponent, AudioSourceComponent, AudioListenerComponent, AudioZoneComponent, \
            AnimatorComponent, ActiveComponent, StaticComponent, \
            UIComponent, UIImageComponent, UITextComponent, UIToggleComponent, UIButtonComponent, UISliderComponent, \
            WorldEnvironmentComponent
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
