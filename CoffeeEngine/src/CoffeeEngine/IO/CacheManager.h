/**
 * @defgroup io IO
 * @brief IO components of the CoffeeEngine.
 * @{
 */

#pragma once

#include <filesystem>

namespace Coffee {

    /**
     * @class CacheManager
     * @brief Manages cache-related operations for the CoffeeEngine.
     */
    class CacheManager
    {
    public:
        /**
         * @brief Sets the cache path.
         * @param path The path to set as the cache directory.
         */
        static void SetCachePath(const std::filesystem::path& path)
        {
            m_cachePath = path;
        }

        /**
         * @brief Gets the current cache path.
         * @return The current cache path.
         */
        static const std::filesystem::path& GetCachePath()
        {
            return m_cachePath;
        }

        /**
         * @brief Creates the cache directory.
         */
        static void CreateCacheDirectory()
        {
            std::filesystem::create_directories(m_cachePath);
        }

        /**
         * @brief Clears the cache by removing all files in the cache directory.
         */
        static void ClearCache()
        {
            std::filesystem::remove_all(m_cachePath);
        }

        /**
         * @brief Gets the file path for a cached file.
         * @param filename The name of the file to be cached.
         * @return The full path to the cached file.
         */
        static std::filesystem::path GetCachedFilePath(const std::string& filename)
        {
            std::filesystem::create_directories(m_cachePath);
            return m_cachePath / (filename);
        }

    private:
        static std::filesystem::path m_cachePath; ///< The path to the cache directory.
    };

}

/** @} */