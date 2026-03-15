#pragma once

#include "CoffeeEngine/Resources/ResourceLoader.h"

namespace Coffee {

    class ResourceImportService
    {
    public:
        explicit ResourceImportService(ResourceLoader* loader = nullptr)
            : m_Loader(loader)
        {
        }

        void SetLoader(ResourceLoader* loader) { m_Loader = loader; }

        template<typename T>
        ResourceRef<T> Import(const ImportData& importData) const
        {
            return m_Loader->Import<T>(importData);
        }

        template<typename T>
        ResourceRef<T> ImportEmbedded(const ImportData& importData) const
        {
            return m_Loader->ImportEmbedded<T>(importData);
        }

        template<typename T>
        ResourceRef<T> ImportFromCache(ResourceID id) const
        {
            return m_Loader->ImportFromCache<T>(id);
        }

    private:
        ResourceLoader* m_Loader = nullptr;
    };

}