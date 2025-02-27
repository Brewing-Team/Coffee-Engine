#pragma once

#include <filesystem>
#include "CoffeeEngine/IO/Resource.h"

namespace Coffee {

    inline ResourceType GetResourceTypeFromExtension(const std::filesystem::path& path)
    {
        auto extension = path.extension();

        if (extension == ".png" || extension == ".jpg" || extension == ".jpeg")
        {
            return ResourceType::Texture2D;
        }
        else if(extension == ".hdr")
        {
            return ResourceType::Cubemap;
        }
        else if(extension == ".glb" || extension == ".gltf" || extension == ".fbx" || extension == ".obj")
        {
            return ResourceType::Model;
        }
        else if(extension == ".glsl")
        {
            return ResourceType::Shader;
        }

        return ResourceType::Unknown;
    }

    inline std::string ResourceTypeToString(ResourceType type)
    {
        switch (type)
        {
        case ResourceType::Texture2D:
            return "Textur2D";
        case ResourceType::Cubemap:
            return "Cubemap";
        case ResourceType::Model:
            return "Model";
        case ResourceType::Mesh:
            return "Mesh";
        case ResourceType::Shader:
            return "Shader";
        case ResourceType::Material:
            return "Material";
        default:
            return "Unknown";
        }
    }

    inline std::string GetResourceExtension(ResourceType type)
    {
        switch (type)
        {
        case ResourceType::Texture2D:
            return ".tex";
        case ResourceType::Cubemap:
            return ".cubemap";
        case ResourceType::Model:
            return ".model";
        case ResourceType::Mesh:
            return ".mesh";
        case ResourceType::Shader:
            return ".shader";
        case ResourceType::Material:
            return ".material";
        default:
            return ".unknown";
        }
    }

    inline ResourceFormat GetResourceSaveFormatFromType(ResourceType type)
    {
        switch (type)
        {
        case Coffee::ResourceType::Unknown:
            break;
        case Coffee::ResourceType::Texture2D:
            return ResourceFormat::Binary;
            break;
        case ResourceType::Cubemap:
            return ResourceFormat::Binary;
            break;
        case Coffee::ResourceType::Model:
            return ResourceFormat::Binary;
            break;
        case Coffee::ResourceType::Mesh:
            return ResourceFormat::Binary;
            break;
        case Coffee::ResourceType::Shader:
            break;
        default:
            return ResourceFormat::Binary;
            break;
        }
    }

}