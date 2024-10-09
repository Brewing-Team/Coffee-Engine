#include "ResourceLoader.h"
#include "CoffeeEngine/Core/Base.h"
#include "CoffeeEngine/IO/Resource.h"

#include "CoffeeEngine/Renderer/Model.h"
#include "CoffeeEngine/Renderer/Shader.h"
#include "CoffeeEngine/Renderer/Texture.h"
#include "CoffeeEngine/IO/ResourceRegistry.h"
#include <filesystem>

namespace Coffee {

    void ResourceLoader::LoadResources(const std::filesystem::path& directory)
    {
        for (const auto& entry : std::filesystem::recursive_directory_iterator(directory))
        {
            if (entry.is_regular_file())
            {
                COFFEE_CORE_INFO("Loading resource {0}", entry.path().string());

                ResourceType type = GetResourceTypeFromExtension(entry.path());
                switch (type)
                {
                    case ResourceType::Texture:
                    {
                        Load<Texture>(entry.path());
                        break;
                    }
                    case ResourceType::Unknown:
                    {
                        COFFEE_CORE_ERROR("ResourceLoader::LoadResources: Unsupported file extension {0}", entry.path().extension().string());
                        break;
                    }
                }            
            }
        }
    }

    Ref<Resource> ResourceLoader::LoadResource(const std::filesystem::path& path)
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