#include "ResourceLoader.h"
#include "CoffeeEngine/Core/Base.h"
#include "CoffeeEngine/IO/Resource.h"

#include "CoffeeEngine/Renderer/Model.h"
#include "CoffeeEngine/Renderer/Texture.h"
#include "CoffeeEngine/IO/ResourceRegistry.h"
#include "CoffeeEngine/IO/ResourceImporter.h"
#include <filesystem>

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

        const std::string& name = path.filename().string();

        if(ResourceRegistry::Exists(name))
        {
            return ResourceRegistry::Get<Texture>(name);
        }

        const Ref<Texture>& texture = s_Importer.ImportTexture(path, srgb, cache);

        ResourceRegistry::Add(name, texture);
        return texture;
    }

    Ref<Model> ResourceLoader::LoadModel(const std::filesystem::path& path, bool cache)
    {
        if(GetResourceTypeFromExtension(path) != ResourceType::Model)
        {
            COFFEE_CORE_ERROR("ResourceLoader::Load<Model>: Resource is not a model!");
            return nullptr;
        }

        const std::string& name = path.filename().string();

        if(ResourceRegistry::Exists(name))
        {
            return ResourceRegistry::Get<Model>(name);
        }

        const Ref<Model>& model = CreateRef<Model>(path);

        ResourceRegistry::Add(name, model);
        return model;
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

}