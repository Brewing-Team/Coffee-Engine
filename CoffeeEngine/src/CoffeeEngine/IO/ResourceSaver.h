#pragma once 

#include "CoffeeEngine/Core/Base.h"
#include "CoffeeEngine/IO/Resource.h"

namespace Coffee
{

    /*
        [x] Create a Save function for saving a resouce on disk
        [x] Create a SaveToCache function that save resource to the project cache
        [ ] Create a way to detect the Resource type and save it in the correct type (Binary or Json)
        [ ] Create private functions to manage the different types of serialization
        [ ] Use this class in the other Resouce Management classes
    */

    enum class ResourceSaveFormat
    {
        Binary,
        JSON
    };

	class ResourceSaver
	{
      public:
        static void Save(const std::filesystem::path& path, const Ref<Resource>& resource);
        static void SaveToCache(const Ref<Resource>& resource);

        static void SetCachePath(const std::filesystem::path& path) { s_cachePath = path; }
      private:
        static void BinarySerialization(const std::filesystem::path&, const Ref<Resource>& resource);
        static void JSONSerialization(const std::filesystem::path&, const Ref<Resource>& resource);
      private:
        static std::filesystem::path s_cachePath;
	};

}