#pragma once

#include "CoffeeEngine/Core/Base.h"
#include "CoffeeEngine/IO/Resource.h"
#include "CoffeeEngine/IO/ResourceFormat.h"
#include "CoffeeEngine/Renderer/Texture.h"
#include <cereal/archives/binary.hpp>
#include <cereal/archives/json.hpp>

namespace Coffee {

    class ResourceImporter
    {
    public:
        Ref<Texture> ImportTexture(const std::filesystem::path& path, bool srgb, bool cache);
    private:
        Ref<Resource> LoadFromCache(const std::filesystem::path& path, ResourceFormat format);

        Ref<Resource> BinaryDeserialization(const std::filesystem::path& path);
        Ref<Resource> JSONDeserialization(const std::filesystem::path& path);
    };
}