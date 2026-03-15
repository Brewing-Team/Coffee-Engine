#pragma once

#include "CoffeeEngine/Resources/ImportData/ImportData.h"

namespace Coffee {

    class IResourceImporter
    {
    public:
        virtual ~IResourceImporter() = default;

        virtual bool CanImport(ResourceType type, const std::filesystem::path& sourcePath) const = 0;
        virtual std::uint32_t GetVersion() const = 0;
    };

}