#pragma once

#include <filesystem>

namespace Coffee {

    class CacheManager
    {
    public:
        static void SetCachePath(const std::filesystem::path& path)
        {
            m_cachePath = path;
        }

        static const std::filesystem::path& GetCachePath()
        {
            return m_cachePath;
        }

        static void CreateCacheDirectory()
        {
            std::filesystem::create_directories(m_cachePath);
        }

        static void ClearCache()
        {
            std::filesystem::remove_all(m_cachePath);
        }

        static std::filesystem::path GetCachedFilePath(const std::string& filename)
        {
            std::filesystem::create_directories(m_cachePath);
            return m_cachePath / (filename + ".res");
        }

    private:
        static std::filesystem::path m_cachePath;
    };

}