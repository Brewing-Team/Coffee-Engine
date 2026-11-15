#pragma once

namespace Coffee
{
    /**
    * @enum ResourceType
    * @brief Enumeration of different types of resources.
    */
    enum class ResourceType
    {
        Unknown, ///< Unknown resource type
        Texture, ///< Texture resource type
        Texture2D, ///< Texture2D resource type
        Texture3D, ///< Texture3D resource type
        Cubemap, ///< Cubemap resource type
        Model,   ///< Model resource type
        Mesh,   ///< Mesh resource type
        Shader,   ///< Shader resource type
        Material, ///< Material resource type
        PBRMaterial, ///< PBRMaterial resource type
        ShaderMaterial, ///< ShaderMaterial resource type
        AnimationSystem, ///< AnimationSystem resource type
        Skeleton, ///< Skeleton resource type
        Animation, ///< Animation resource type
        AnimationController, ///< AnimationController resource type
        Prefab, ///< Prefab resource type
    };
}