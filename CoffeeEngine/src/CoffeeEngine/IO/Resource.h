#pragma once
#include "CoffeeEngine/Core/Assert.h"
#include <cereal/access.hpp>
#include <cereal/archives/binary.hpp>
#include <filesystem>
#include "CoffeeEngine/IO/Serialization/FilesystemPathSerialization.h"
#include <cereal/types/polymorphic.hpp>

namespace Coffee {

    enum class ResourceType
    {
        Unknown,
        Texture,
        Shader
    };

    class Resource
    {
    public:
       /*  Resource(const std::filesystem::path& path)
            : m_FilePath(path), m_Name(path.filename()) {} */
        //Resource() = default;
        Resource(ResourceType type = ResourceType::Unknown)
            : m_Type(type) {}
        virtual ~Resource() = default;

        const std::string& GetName() const { return m_Name; }
        const std::filesystem::path& GetPath() { COFFEE_CORE_ASSERT(m_FilePath.empty(), "This Texture does not exist on disk!") return m_FilePath; };
        ResourceType GetType() const { return m_Type; }
    private:
        friend class cereal::access;

        template <class Archive>
        void save(Archive& archive) const
        {
            int typeInt = static_cast<int>(m_Type);
            archive(m_Name, m_FilePath, typeInt);
        }

        template <class Archive>
        void load(Archive& archive)
        {
            int typeInt;
            archive(m_Name, m_FilePath, typeInt);
            m_Type = static_cast<ResourceType>(typeInt);
        }
    
    protected:
        std::string m_Name;
        std::filesystem::path m_FilePath;
        ResourceType m_Type;
    };

}

CEREAL_REGISTER_TYPE(Coffee::Resource);