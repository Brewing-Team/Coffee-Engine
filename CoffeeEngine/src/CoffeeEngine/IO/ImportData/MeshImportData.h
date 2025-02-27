#pragma once

#include "CoffeeEngine/IO/ImportData/ImportData.h"
#include <cereal/cereal.hpp>

namespace Coffee
{

    struct MeshImportData : public ImportData
    {



        MeshImportData() : ImportData(ResourceType::Mesh) {}

        template <typename Archive>
        void serialize(Archive& archive)
        {
            archive(cereal::base_class<ImportData>(this));
        }
    };

} // namespace Coffee