#pragma once

#include "CoffeeEngine/IO/ImportData/ImportData.h"
#include <cereal/cereal.hpp>

namespace Coffee
{

    struct Texture2DImportData : public ImportData
    {
        bool sRGB = true;

        Texture2DImportData() : ImportData(ResourceType::Texture2D) {}

        template <typename Archive>
        void serialize(Archive& archive)
        {
            archive(cereal::base_class<ImportData>(this));
        }
    };

} // namespace Coffee