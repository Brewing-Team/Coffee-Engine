#pragma once

#include "CoffeeEngine/IO/ImportData/CubemapImportData.h"
#include "CoffeeEngine/IO/ImportData/ImportData.h"
#include "CoffeeEngine/IO/ImportData/MaterialImportData.h"
#include "CoffeeEngine/IO/ImportData/ModelImportData.h"
#include "CoffeeEngine/IO/ImportData/ShaderImportData.h"
#include "CoffeeEngine/IO/ImportData/Texture2DImportData.h"
#include "CoffeeEngine/IO/Resource.h"

namespace Coffee {

    template<typename T>
    static Scope<ImportData> CreateImportData()
    {
        switch(GetResourceType<T>())
        {
            case ResourceType::Texture2D:
            {
                return CreateScope<Texture2DImportData>();
            }
            case ResourceType::Cubemap:
            {
                return CreateScope<CubemapImportData>();
            }
            case ResourceType::Model:
            {
                return CreateScope<ModelImportData>();
            }
            case ResourceType::Material:
            {
                return CreateScope<MaterialImportData>();
            }
            case ResourceType::Shader:
            {
                return CreateScope<ShaderImportData>();
            }
            default:
            {
                COFFEE_CORE_ERROR("ResourceLoader::CreateImportData: Unsupported resource type.");
                return CreateScope<ImportData>();
            }
        }
    }


}