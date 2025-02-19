#pragma once

#include "CoffeeEngine/Core/UUID.h"

#include <cereal/cereal.hpp>
#include <filesystem>
#include <CoffeeEngine/IO/Resource.h>

namespace Coffee {

    struct ImportData
    {
        UUID uuid;
        ResourceType type;

        std::filesystem::path originalPath;
        std::filesystem::path cachedPath;

        template<typename Archive>
        void serialize(Archive& archive)
        {
            archive(CEREAL_NVP(uuid), CEREAL_NVP(type), CEREAL_NVP(originalPath), CEREAL_NVP(cachedPath));
        }
    };

}