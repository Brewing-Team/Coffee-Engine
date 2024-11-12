#include "ResourceLoader.h"
#include "CoffeeEngine/Core/Base.h"
#include "CoffeeEngine/IO/Resource.h"

#include "CoffeeEngine/Renderer/Model.h"
#include "CoffeeEngine/Renderer/Shader.h"
#include "CoffeeEngine/Renderer/Texture.h"
#include "CoffeeEngine/IO/ResourceRegistry.h"
#include "CoffeeEngine/IO/ResourceImporter.h"
#include <cstdint>
#include <filesystem>
#include <fstream>

namespace Coffee {

    ResourceImporter ResourceLoader::s_Importer = ResourceImporter();

    void ResourceLoader::LoadFile(const std::filesystem::path& path)
    {
        ResourceType type = GetResourceTypeFromExtension(path);
        switch (type)
        {
            case ResourceType::Texture:
            {
                LoadTexture(path);
                break;
            }
            case ResourceType::Model:
            {
                LoadModel(path, false);
                break;
            }
            case ResourceType::Unknown:
            {
                COFFEE_CORE_ERROR("ResourceLoader::LoadResources: Unsupported file extension {0}", path.extension().string());
                break;
            }
        }
    }

    void ResourceLoader::LoadDirectory(const std::filesystem::path& directory)
    {
        for (const auto& entry : std::filesystem::recursive_directory_iterator(directory))
        {
            if (entry.is_regular_file())
            {
                COFFEE_CORE_INFO("Loading resource {0}", entry.path().string());

                LoadFile(entry.path());
            }
        }
    }

    /* Ref<Resource> ResourceLoader::LoadResource(const std::filesystem::path& path)
    {
        auto extension = path.extension();

        if (extension == ".png" || extension == ".jpg" || extension == ".jpeg")
        {
            return Texture::Load(path);
        }
        else if(extension == ".glb" || extension == ".gltf" || extension == ".fbx" || extension == ".obj")
        {
            return Model::Load(path);
        }
        //When the frag and vert shaders are merged into a single file called .glsl change this to .glsl
        //FIXME: The shader class does not add the Shader to the registry
        else if(extension == ".frag" || extension == ".vert")
        {
            if(extension == ".frag")
            {
                auto vertPath = path.parent_path() / (path.stem().string() + ".vert");
                return Shader::Create(vertPath.string(), path.string());
            }
            else if(extension == ".vert")
            {
                auto fragPath = path.parent_path() / (path.stem().string() + ".frag");
                return Shader::Create(path.string(), fragPath.string());
            }
        }
        
        COFFEE_CORE_ERROR("ResourceLoader::LoadResource: Unsupported file extension {0}", extension.string());
        return nullptr;
    } */

    Ref<Texture> ResourceLoader::LoadTexture(const std::filesystem::path& path, bool srgb, bool cache)
    {
        if(GetResourceTypeFromExtension(path) != ResourceType::Texture)
        {
            COFFEE_CORE_ERROR("ResourceLoader::Load<Texture>: Resource is not a texture!");
            return nullptr;
        }

        UUID uuid = GetUUIDFromImportFile(path);

        if(ResourceRegistry::Exists(uuid))
        {
            return ResourceRegistry::Get<Texture>(uuid);
        }

        const Ref<Texture>& texture = s_Importer.ImportTexture(path, srgb, cache);

        ResourceRegistry::Add(uuid, texture);
        return texture;
    }

    Ref<Model> ResourceLoader::LoadModel(const std::filesystem::path& path, bool cache)
    {
        if(GetResourceTypeFromExtension(path) != ResourceType::Model)
        {
            COFFEE_CORE_ERROR("ResourceLoader::Load<Model>: Resource is not a model!");
            return nullptr;
        }

        UUID uuid = GetUUIDFromImportFile(path);

        if(ResourceRegistry::Exists(uuid))
        {
            return ResourceRegistry::Get<Model>(uuid);
        }

        const Ref<Model>& model = CreateRef<Model>(path);

        ResourceRegistry::Add(uuid, model);
        return model;
    }

    Ref<Shader> ResourceLoader::LoadShader(const std::filesystem::path& vertexPath, const std::filesystem::path& fragmentPath)
    {
        if(GetResourceTypeFromExtension(vertexPath) != ResourceType::Shader || GetResourceTypeFromExtension(fragmentPath) != ResourceType::Shader)
        {
            COFFEE_CORE_ERROR("ResourceLoader::Load<Shader>: Resource is not a shader!");
            return nullptr;
        }

        //TODO: Add support for Resource Registry, Resource Importer and UUIDs

        //OLD CODE
        /*
            std::filesystem::path filePath(vertexPath);
            std::string fileName = filePath.stem().string();

            if(ResourceRegistry::Exists(fileName))
            {
                return ResourceRegistry::Get<Shader>(fileName);
            }
            else
            {
                Ref<Shader> shader = CreateRef<Shader>(vertexPath, fragmentPath);
                ResourceRegistry::Add(fileName, shader);
                return shader;
            }
        */
        
        return CreateRef<Shader>(vertexPath, fragmentPath);
    }

    ResourceType ResourceLoader::GetResourceTypeFromExtension(const std::filesystem::path& path)
    {
        auto extension = path.extension();

        if (extension == ".png" || extension == ".jpg" || extension == ".jpeg")
        {
            return ResourceType::Texture;
        }
        else if(extension == ".glb" || extension == ".gltf" || extension == ".fbx" || extension == ".obj")
        {
            return ResourceType::Model;
        }
        else if(extension == ".frag" || extension == ".vert")
        {
            return ResourceType::Shader;
        }

        return ResourceType::Unknown;
    }

    UUID ResourceLoader::GetUUIDFromImportFile(const std::filesystem::path& path)
    {
        UUID uuid;

        std::filesystem::path importFilePath = path;
        importFilePath.replace_extension(".import");

        if(std::filesystem::exists(importFilePath))
        {
            std::ifstream importFile(importFilePath);
            cereal::JSONInputArchive archive(importFile);
            archive(uuid);
        }
        else
        {
            std::ofstream importFile(importFilePath);
            cereal::JSONOutputArchive archive(importFile);
            archive(uuid);
        }

        return uuid;
    }
}