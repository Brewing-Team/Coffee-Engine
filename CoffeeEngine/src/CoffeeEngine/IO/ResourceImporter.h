#pragma once

#include "CoffeeEngine/Project/Project.h"
#include <cereal/archives/binary.hpp>
#include <memory>
#include <fstream>

namespace Coffee {

    class ResourceImporter
    {
    public:
        template<typename T, typename... Args>
        static Ref<T> Import(bool cache, const std::filesystem::path& path, Args&&... args)
        {
            if(!cache)
            {
                return LoadFromFile<T>(path, std::forward<Args>(args)...);
            }

            std::filesystem::create_directories(m_cachePath);

            std::filesystem::path cachedFilePath = m_cachePath / (path.filename().stem() += ".res");

            if (std::filesystem::exists(cachedFilePath))
            {
                return std::static_pointer_cast<T>(LoadFromCache<T>(cachedFilePath));
            }
            else
            {
                Ref<T> resource = LoadFromFile<T>(path, std::forward<Args>(args)...);
                SaveToCache(cachedFilePath, resource);
                return resource;
            }
        }

        static void SetCachePath(const std::filesystem::path& path)
        {
            m_cachePath = path;
        }
    private:
        template<typename T, typename... Args>
        static Ref<T> LoadFromFile(const std::filesystem::path& path, Args&&... args)
        {
            return CreateRef<T>(path, std::forward<Args>(args)...);
        }

        template<typename T>
        static void SaveToCache(const std::filesystem::path& path, Ref<T> resource)
        {
            std::ofstream file{path, std::ios::binary};
            cereal::BinaryOutputArchive oArchive(file);
            oArchive(resource);
        }

        template<typename T>
        static Ref<T> LoadFromCache(const std::filesystem::path& path)
        {
            std::ifstream file(path, std::ios::binary);
            cereal::BinaryInputArchive archive(file);
            Ref<T> resource = CreateRef<T>();
            archive(resource);
            return resource;
        }
    private:
        static std::filesystem::path m_cachePath;
    };
}