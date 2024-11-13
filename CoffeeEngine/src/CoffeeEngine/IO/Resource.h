/**
 * @defgroup io IO
 * @brief IO components of the CoffeeEngine.
 * @{
 */

#pragma once
#include "CoffeeEngine/Core/Assert.h"
#include <cereal/access.hpp>
#include <cereal/archives/binary.hpp>
#include <filesystem>
#include "CoffeeEngine/IO/Serialization/FilesystemPathSerialization.h"
#include <cereal/types/polymorphic.hpp>

namespace Coffee {

    /**
     * @enum ResourceType
     * @brief Enumeration of different types of resources.
     */
    enum class ResourceType
    {
        Unknown, ///< Unknown resource type
        Texture, ///< Texture resource type
        Model,   ///< Model resource type
        Mesh,   ///< Mesh resource type
        Shader,   ///< Shader resource type
    };

    /**
     * @class Resource
     * @brief Base class for different types of resources in the CoffeeEngine.
     */
    class Resource
    {
    public:
        /**
         * @brief Constructor that initializes the resource with a type.
         * @param type The type of the resource.
         */
        Resource(ResourceType type = ResourceType::Unknown)
            : m_Type(type) {}

        /**
         * @brief Virtual destructor.
         */
        virtual ~Resource() = default;

        /**
         * @brief Gets the name of the resource.
         * @return The name of the resource.
         */
        const std::string& GetName() const { return m_Name; }

        /**
         * @brief Gets the file path of the resource.
         * @return The file path of the resource.
         */
        const std::filesystem::path& GetPath() { COFFEE_CORE_ASSERT(m_FilePath.empty(), "This Texture does not exist on disk!") return m_FilePath; }

        /**
         * @brief Gets the type of the resource.
         * @return The type of the resource.
         */
        ResourceType GetType() const { return m_Type; }

    private:
        friend class cereal::access;

        /**
         * @brief Serializes the resource to an archive.
         * @tparam Archive The type of the archive.
         * @param archive The archive to save the resource to.
         */
        template <class Archive>
        void save(Archive& archive) const
        {
            int typeInt = static_cast<int>(m_Type);
            archive(m_Name, m_FilePath, typeInt);
        }

        /**
         * @brief Deserializes the resource from an archive.
         * @tparam Archive The type of the archive.
         * @param archive The archive to load the resource from.
         */
        template <class Archive>
        void load(Archive& archive)
        {
            int typeInt;
            archive(m_Name, m_FilePath, typeInt);
            m_Type = static_cast<ResourceType>(typeInt);
        }

    protected:
        std::string m_Name; ///< The name of the resource.
        std::filesystem::path m_FilePath; ///< The file path of the resource.
        ResourceType m_Type; ///< The type of the resource.
    };

}

CEREAL_REGISTER_TYPE(Coffee::Resource);

/** @} */