#pragma once
#include "CoffeeEngine/Core/Assert.h"
#include <filesystem>

namespace Coffee {

    class Resource
    {
    public:
       /*  Resource(const std::filesystem::path& path)
            : m_FilePath(path), m_Name(path.filename()) {} */
        Resource() = default;
        virtual ~Resource() = default;

        const std::string& GetName() const { return m_Name; }
        const std::filesystem::path& GetPath() { COFFEE_CORE_ASSERT(m_FilePath.empty(), "This Texture does not exist on disk!") return m_FilePath; };
    protected:
        std::string m_Name;
        std::filesystem::path m_FilePath;
    };

}