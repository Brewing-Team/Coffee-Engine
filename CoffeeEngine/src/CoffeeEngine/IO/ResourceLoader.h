#pragma once 

#include "CoffeeEngine/IO/Resource.h"
#include "CoffeeEngine/IO/ResourceRegistry.h"
#include "CoffeeEngine/IO/ResourceImporter.h"
#include <filesystem>
#include <string>
namespace Coffee {

    class ResourceLoader
    {
    public:
        static void LoadResources(const std::filesystem::path& directory);
        static Ref<Resource> LoadResource(const std::filesystem::path& path);

        template <typename T, typename... Args>
        static Ref<T> Load(const std::filesystem::path& path, Args&&... args)
        {
            //TODO: Check if the specified type (T) is a known resource type and is equals to the resource type of the file


            if(GetResourceTypeFromExtension(path) == ResourceType::Unknown)
            {
                COFFEE_CORE_ERROR("ResourceLoader::Load: Resource is of unknown type!");
                return nullptr;
            }

            const std::string& name = path.filename().string();

            if(ResourceRegistry::Exists(name))
            {
                return ResourceRegistry::Get<T>(name);
            }

            const Ref<T>& resource = ResourceImporter::Import<T>(path, std::forward<Args>(args)...);

            if (resource->GetType() != GetResourceTypeFromExtension(path))
            {
                COFFEE_CORE_ERROR("ResourceLoader::Load: Resource is not of the specified type!");
                return nullptr;
            }

            ResourceRegistry::Add(name, resource);

            return resource;
        }
    private:
        static ResourceType GetResourceTypeFromExtension(const std::filesystem::path& path);
    };

}