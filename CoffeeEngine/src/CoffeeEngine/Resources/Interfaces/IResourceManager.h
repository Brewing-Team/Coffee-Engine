#pragma once

#include "CoffeeEngine/Resources/Resource.h"

#include <future>
#include <optional>

namespace Coffee {

    class IResourceManager
    {
    public:
        virtual ~IResourceManager() = default;

        virtual ResourceHandle LoadResource(const std::filesystem::path& path, ResourceType type) = 0;
        virtual std::future<ResourceHandle> LoadResourceAsync(const std::filesystem::path& path, ResourceType type) = 0;

        virtual ResourceLoadState GetLoadState(ResourceID id) const = 0;
    };

}