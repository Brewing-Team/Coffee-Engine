#pragma once

#include "CoffeeEngine/Core/UUID.h"

#include <cereal/cereal.hpp>
#include <filesystem>
#include <CoffeeEngine/IO/Resource.h>

namespace Coffee {

    struct ImportData
    {
        UUID uuid = UUID::null;
        ResourceType type = ResourceType::Unknown;

        std::filesystem::path originalPath;
        std::filesystem::path cachedPath;

        ImportData() = default;
        ImportData(ResourceType type) : type(type) {}

        template<typename Archive>
        void serialize(Archive& archive)
        {
            archive(CEREAL_NVP(uuid), CEREAL_NVP(type), CEREAL_NVP(originalPath), CEREAL_NVP(cachedPath));
        }

        bool IsValid() const
        {
            // Implement the logic to check if the ImportData has the necessary embedded data
            // This is a placeholder implementation
            return uuid != UUID::null && !cachedPath.empty();
        }
    };

}