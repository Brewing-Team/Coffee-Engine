#pragma once

#include "CoffeeEngine/Core/UUID.h"

#include <cereal/cereal.hpp>
#include <filesystem>

namespace Coffee {

    struct ImportData
    {
        UUID uuid;
        std::filesystem::path originalPath;

        template<typename Archive>
        void serialize(Archive& archive)
        {
            archive(CEREAL_NVP(uuid), CEREAL_NVP(originalPath));
        }
    };

}