#pragma once

#include "CoffeeEngine/IO/ResourceUtils.h"

#include <filesystem>

namespace Coffee {

    class ImportData;

    namespace ImportDataUtils
    {
        void SaveImportData(Scope<ImportData>& importData);
        // TODO: Think if the path should be the .import path or the resource and replace the extension inside the function
        Scope<ImportData> LoadImportData(const std::filesystem::path& path);
        bool HasImportFile(const std::filesystem::path& path);

        Scope<ImportData> CreateImportData(ResourceType type);
    
        template<typename T>
        Scope<ImportData> CreateImportData()
        {
            ResourceType type = GetResourceType<T>();
            return CreateImportData(type);
        }
    }

}