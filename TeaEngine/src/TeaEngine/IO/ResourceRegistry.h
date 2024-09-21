#pragma once

#include "TeaEngine/Core/Base.h"
#include "TeaEngine/IO/Resource.h"
#include <string>
#include <unordered_map>
namespace Tea {

    class ResourceRegistry
    {
    public:
        static void Add(const std::string& name, Ref<Resource> resource) { m_Resources[name] = resource; }
        template<typename T>
        static Ref<T> Get(const std::string& name)
        {
            if (m_Resources.find(name) == m_Resources.end())
            {
                TEA_CORE_ERROR("Resource {0} not found!", name.c_str());
                return nullptr;
            }
            return std::static_pointer_cast<T>(m_Resources[name]);
        }
        static bool Exists(const std::string& name) { return m_Resources.find(name) != m_Resources.end(); }

        //For debugging purposes only
        static const std::unordered_map<std::string, Ref<Resource>>& GetResourceRegistry() { return m_Resources; }
    private:
        static std::unordered_map<std::string, Ref<Resource>> m_Resources;
    };
}