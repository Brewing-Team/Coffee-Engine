#include "ResourceLoader.h"
#include "CoffeeEngine/Core/Base.h"
#include "CoffeeEngine/Renderer/Model.h"
#include "CoffeeEngine/Renderer/Shader.h"
#include "CoffeeEngine/Renderer/Texture.h"
#include <filesystem>

namespace Coffee {

    void ResourceLoader::LoadResources(const std::filesystem::path& directory)
    {
        for (const auto& entry : std::filesystem::recursive_directory_iterator(directory))
        {
            if (entry.is_regular_file())
            {
                //I don't know if it's better to add the resource to the registry here or in the Load function of the resource
            
                //Commented implementation of adding the resource to the registry here

                //------------------------------------------------------------------//

                /* auto resource = LoadResource(entry.path());
                if (resource)
                {
                    std::string resourceName = entry.path().filename().string();
                    ResourceRegistry::Add(resourceName, resource);
                } */

                //------------------------------------------------------------------//

                //Any Resource should be loaded using ResourceLoader::LoadResource instead of using the Load function of the resource
            
            
                //==================================================================//

                //Using the Load function of the resource

                COFFEE_CORE_INFO("Loading resource {0}", entry.path().string());

                ResourceLoader::LoadResource(entry.path());
            
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

}