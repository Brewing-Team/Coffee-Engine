#pragma once
#include "TeaEngine/Core/Assert.h"
#include <filesystem> // Add this line to include the std::filesystem header

namespace Tea {

    class Resource
    {
    public:
       /*  Resource(const std::filesystem::path& path)
            : m_FilePath(path), m_Name(path.filename()) {} */
        Resource() = default;
        virtual ~Resource() = default;

        const std::string& GetName() const { return m_Name; }
        const std::string& GetPath() { TEA_CORE_ASSERT(m_FilePath.empty(), "This Texture does not exist on disk!") return m_FilePath; };
    protected:
        std::string m_Name;
        std::filesystem::path m_FilePath;
    };

}