#pragma once 

#include "CoffeeEngine/IO/Resource.h"
#include <filesystem>
namespace Coffee {

    class ResourceLoader
    {
    public:
        static void LoadResources(const std::filesystem::path& directory);
        static Ref<Resource> LoadResource(const std::filesystem::path& path);
    private:
    };

}