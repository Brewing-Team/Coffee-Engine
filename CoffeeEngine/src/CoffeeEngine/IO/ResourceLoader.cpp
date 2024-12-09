#include "ResourceLoader.h"
#include "CoffeeEngine/Core/Base.h"
#include "CoffeeEngine/Core/Log.h"
#include "CoffeeEngine/IO/Resource.h"
#include "CoffeeEngine/Renderer/Material.h"
#include "CoffeeEngine/Renderer/Model.h"
#include "CoffeeEngine/Renderer/Shader.h"
#include "CoffeeEngine/Renderer/Texture.h"
#include "CoffeeEngine/IO/ResourceRegistry.h"
#include "CoffeeEngine/IO/ResourceImporter.h"
#include "CoffeeEngine/IO/ResourceUtils.h"
#include <filesystem>
#include <fstream>
#include <string>

namespace Coffee {

    std::filesystem::path ResourceLoader::s_WorkingDirectory = std::filesystem::current_path();
    ResourceImporter ResourceLoader::s_Importer = ResourceImporter();

    void ResourceLoader::LoadFile(const std::filesystem::path& path)
    {
        if (!is_regular_file(path))
        {
            COFFEE_CORE_ERROR("ResourceLoader::LoadResources: {0} is not a file!", path.string());
            return;
        }

        const ResourceType type = GetResourceTypeFromExtension(path);

        if(type == ResourceType::Unknown and path.extension() != ".import")
        {
            COFFEE_CORE_ERROR("ResourceLoader::LoadResources: Unsupported file extension {0}", path.extension().string());
            return;
        }

        std::filesystem::path resourcePath = path;

        if(resourcePath.extension() == ".import")
        {
            resourcePath = GetPathFromImportFile(path);
            COFFEE_CORE_INFO("ResourceLoader::LoadDirectory: Loading resource from import file {0}", resourcePath.string());
        }
        else
        {
            std::filesystem::path importFilePath = resourcePath;
            importFilePath.replace_extension(".import");
            if(!std::filesystem::exists(importFilePath))
            {
                COFFEE_CORE_INFO("ResourceLoader::LoadDirectory: Generating import file for {0}", resourcePath.string());
                GenerateImportFile(resourcePath);
            }
        }

        switch (type)
        {
            case ResourceType::Texture2D:
            {
                LoadTexture2D(path);
                break;
            }
            case ResourceType::Cubemap:
            {
                LoadCubemap(path);
                break;
            }
            case ResourceType::Model:
            {
                LoadModel(path);
                break;
            }
            case ResourceType::Shader:
            {
                LoadShader(path);
                break;
            }
        }
    }

    void ResourceLoader::LoadDirectory(const std::filesystem::path& directory)
    {
        for (const auto& entry : std::filesystem::recursive_directory_iterator(directory))
        {
            // This two if statements are duplicated in LoadFile but are necessary to suppress errors

            if (!entry.is_regular_file())
            {
                continue;
            }

            if(GetResourceTypeFromExtension(entry.path()) == ResourceType::Unknown and entry.path().extension() != ".import")
            {
                continue;
            }

            LoadFile(entry.path());
        }
    }

    Ref<Texture2D> ResourceLoader::LoadTexture2D(const std::filesystem::path& path, bool srgb, bool cache)
    {
        if(GetResourceTypeFromExtension(path) != ResourceType::Texture2D)
        {
            COFFEE_CORE_ERROR("ResourceLoader::Load<Texture2D>: Resource is not a texture!");
            return nullptr;
        }

        UUID uuid = GetUUIDFromImportFile(path);

        if(ResourceRegistry::Exists(uuid))
        {
            return ResourceRegistry::Get<Texture2D>(uuid);
        }

        const Ref<Texture2D>& texture = s_Importer.ImportTexture2D(path, uuid, srgb, cache);
        texture->SetUUID(uuid);

        ResourceRegistry::Add(uuid, texture);
        return texture;
    }

    Ref<Texture2D> ResourceLoader::LoadTexture2D(UUID uuid)
    {
        if(uuid == UUID::null)
            return nullptr;

        if(ResourceRegistry::Exists(uuid))
        {
            return ResourceRegistry::Get<Texture2D>(uuid);
        }

        const Ref<Texture2D>& texture = s_Importer.ImportTexture2D(uuid);

        ResourceRegistry::Add(uuid, texture);
        return texture;
    }

    Ref<Cubemap> ResourceLoader::LoadCubemap(const std::filesystem::path& path)
    {
        if(GetResourceTypeFromExtension(path) != ResourceType::Cubemap)
        {
            COFFEE_CORE_ERROR("ResourceLoader::Load<Cubemap>: Resource is not a cubemap!");
            return nullptr;
        }

        UUID uuid = GetUUIDFromImportFile(path);

        if(ResourceRegistry::Exists(uuid))
        {
            return ResourceRegistry::Get<Cubemap>(uuid);
        }

        const Ref<Cubemap>& cubemap = s_Importer.ImportCubemap(path, uuid);
        cubemap->SetUUID(uuid);
        cubemap->SetName(path.filename().string());

        ResourceRegistry::Add(uuid, cubemap);
        return cubemap;
    }
    Ref<Cubemap> ResourceLoader::LoadCubemap(UUID uuid)
    {
        return nullptr;
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

        const Ref<Model>& model = s_Importer.ImportModel(path, cache);
        model->SetUUID(uuid);

        ResourceRegistry::Add(uuid, model);
        return model;
    }

    Ref<Mesh> ResourceLoader::LoadMesh(const std::string& name, const std::vector<Vertex>& vertices, const std::vector<uint32_t>& indices, Ref<Material>& material)
    {
        if(ResourceRegistry::Exists(name))
        {
            return ResourceRegistry::Get<Mesh>(name);
        }
        
        UUID uuid = ResourceRegistry::GetUUIDByName(name);

        const Ref<Mesh>& mesh = s_Importer.ImportMesh(name, uuid, vertices, indices, material);
        mesh->SetName(name);

        ResourceRegistry::Add(uuid, mesh);
        return mesh;
    }

    Ref<Mesh> ResourceLoader::LoadMesh(UUID uuid)
    {
        if(ResourceRegistry::Exists(uuid))
        {
            return ResourceRegistry::Get<Mesh>(uuid);
        }

        const Ref<Mesh>& mesh = s_Importer.ImportMesh(uuid);

        ResourceRegistry::Add(uuid, mesh);
        return mesh;
    }

    Ref<Shader> ResourceLoader::LoadShader(const std::filesystem::path& shaderPath)
    {
        if(ResourceRegistry::Exists(name))
        {
            return ResourceRegistry::Get<Mesh>(name);
        }

        const Ref<Mesh>& mesh = s_Importer.ImportMesh(name, vertices, indices);

        if(ResourceRegistry::Exists(uuid))
        {
            return ResourceRegistry::Get<Shader>(uuid);
        }

        const Ref<Shader>& shader = CreateRef<Shader>(shaderPath);
        shader->SetUUID(uuid);

        ResourceRegistry::Add(uuid, shader);

        return shader;
    }

    Ref<Material> ResourceLoader::LoadMaterial(const std::string& name)
    {
        std::string materialName = name;

        UUID uuid;

        if(materialName.empty())
        {
            materialName = "Material-" + std::to_string(uuid);
        }

        if(ResourceRegistry::Exists(materialName))
        {
            return ResourceRegistry::Get<Material>(materialName);
        }

        Ref<Material> material = s_Importer.ImportMaterial(materialName, uuid);
        material->SetUUID(uuid);
        ResourceRegistry::Add(uuid, material);
        return material;

    }

    Ref<Material> ResourceLoader::LoadMaterial(const std::string& name, MaterialTextures& materialTextures)
    {
        std::string materialName = name;

        UUID uuid;

        if(materialName.empty())
        {
            materialName = "Material-" + std::to_string(uuid);
        }

        if(ResourceRegistry::Exists(materialName))
        {
            return ResourceRegistry::Get<Material>(materialName);
        }

        Ref<Material> material = s_Importer.ImportMaterial(materialName, uuid, materialTextures);
        material->SetUUID(uuid);
        ResourceRegistry::Add(uuid, material);
        return material;
    }
    
    Ref<Material> ResourceLoader::LoadMaterial(UUID uuid)
    {
        if(ResourceRegistry::Exists(uuid))
        {
            return ResourceRegistry::Get<Material>(uuid);
        }

        const Ref<Material>& material = s_Importer.ImportMaterial(uuid);

        ResourceRegistry::Add(uuid, material);
        return material;
    }

    void ResourceLoader::GenerateImportFile(const std::filesystem::path& path)
    {
        std::filesystem::path importFilePath = path;
        importFilePath.replace_extension(".import");

        if(!std::filesystem::exists(importFilePath))
        {
            ImportData importData;
            importData.uuid = UUID();
            std::filesystem::path relativePath = std::filesystem::relative(path, s_WorkingDirectory);
            importData.originalPath = relativePath;

            std::ofstream importFile(importFilePath);
            cereal::JSONOutputArchive archive(importFile);
            archive(CEREAL_NVP(importData));
        }
    }

    ResourceLoader::ImportData ResourceLoader::GetImportData(const std::filesystem::path& path)
    {
        ImportData importData;

        std::filesystem::path importFilePath = path;
        importFilePath.replace_extension(".import");

        /* TODO: add the path serialization relative to the project directory.
         * This means that when we load the path from the .import file we need to append the project directory
         * 
        */

        if(std::filesystem::exists(importFilePath))
        {
            std::ifstream importFile(importFilePath);
            cereal::JSONInputArchive archive(importFile);
            archive(CEREAL_NVP(importData));

            // Convert the relative path to an absolute path
            importData.originalPath = s_WorkingDirectory / importData.originalPath;
        }
        else
        {
            COFFEE_CORE_ERROR("ResourceLoader::GetImportData: .import file does not exist for {0}", path.string());
        }

        return importData;
    }

    UUID ResourceLoader::GetUUIDFromImportFile(const std::filesystem::path& path)
    {
        ImportData importData = GetImportData(path);
        return importData.uuid;
    }

    std::filesystem::path ResourceLoader::GetPathFromImportFile(const std::filesystem::path& path)
    {
        ImportData importData = GetImportData(path);
        return importData.originalPath;
    }
}