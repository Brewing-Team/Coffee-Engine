#pragma once

#include "CoffeeEngine/Core/Base.h"
#include "CoffeeEngine/IO/Resource.h"
#include "CoffeeEngine/IO/ResourceFormat.h"
#include "CoffeeEngine/IO/ResourceSaver.h"
#include "CoffeeEngine/Project/Project.h"
#include "CoffeeEngine/Renderer/Texture.h"
#include <cereal/archives/binary.hpp>
#include <cereal/archives/json.hpp>
#include <memory>
#include <fstream>

namespace Coffee {

    class ResourceImporter
    {
    public:
        static Ref<Texture> ImportTexture(const std::filesystem::path& path, bool srgb, bool cache);
    private:
        static Ref<Resource> LoadFromCache(const std::filesystem::path& path, ResourceFormat format);

        static Ref<Resource> BinaryDeserialization(const std::filesystem::path& path);
        static Ref<Resource> JSONDeserialization(const std::filesystem::path& path);
    };
}