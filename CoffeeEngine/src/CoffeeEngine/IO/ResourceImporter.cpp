#include "ResourceImporter.h"
#include "CoffeeEngine/Renderer/Material.h"
#include "ResourceSaver.h"
#include "CoffeeEngine/IO/CacheManager.h"
#include "CoffeeEngine/Renderer/Model.h"
#include "CoffeeEngine/Renderer/Mesh.h"
#include "CoffeeEngine/Renderer/Material.h"

#include <cstdint>
#include <filesystem>
#include <fstream>
#include <string>

namespace Coffee {

    Ref<Texture> ResourceImporter::ImportTexture(const std::filesystem::path& path, bool srgb, bool cache)
    {
        if (!cache)
        {
            return CreateRef<Texture>(path, srgb);
        }

        std::filesystem::path cachedFilePath = CacheManager::GetCachedFilePath(path.filename().string());

        if (std::filesystem::exists(cachedFilePath))
        {
            const Ref<Resource>& resource = LoadFromCache(cachedFilePath, ResourceFormat::Binary);
            return std::static_pointer_cast<Texture>(resource);
        }
        else
        {
            COFFEE_WARN("ResourceImporter::ImportTexture: Texture {0} not found in cache. Creating new texture.", path.string());
            Ref<Texture> texture = CreateRef<Texture>(path, srgb);
            ResourceSaver::SaveToCache(texture->GetName(), texture); //TODO: Add the UUID to the cache filename
            return texture;
        }
    }

    Ref<Model> ResourceImporter::ImportModel(const std::filesystem::path& path, bool cache)
    {
        if (!cache)
        {
            return CreateRef<Model>(path);
        }

        std::filesystem::path cachedFilePath = CacheManager::GetCachedFilePath(path.filename().string());

        if (std::filesystem::exists(cachedFilePath))
        {
            const Ref<Resource>& resource = LoadFromCache(cachedFilePath, ResourceFormat::Binary);
            return std::static_pointer_cast<Model>(resource);
        }
        else
        {
            COFFEE_WARN("ResourceImporter::ImportModel: Model {0} not found in cache. Creating new model.", path.string());
            Ref<Model> model = CreateRef<Model>(path);
            ResourceSaver::SaveToCache(model->GetName(), model);
            return model;
        }
    }

    Ref<Mesh> ResourceImporter::ImportMesh(const std::string& name, const UUID& uuid, const std::vector<Vertex>& vertices, const std::vector<uint32_t>& indices, Ref<Material>& material)
    {
        // TODO: Think about adding a cache parameter.

        std::string uuidString = std::to_string(uuid);

        std::filesystem::path cachedFilePath = CacheManager::GetCachedFilePath(uuidString);

        if(std::filesystem::exists(cachedFilePath))
        {
            const Ref<Resource>& resource = LoadFromCache(cachedFilePath, ResourceFormat::Binary);
            return std::static_pointer_cast<Mesh>(resource);
        }
        else
        {
            COFFEE_WARN("ResourceImporter::ImportMesh: Mesh {0} not found in cache. Creating new mesh.", (uint64_t)uuid);
            Ref<Mesh> mesh = CreateRef<Mesh>(vertices, indices);
            mesh->SetUUID(uuid);
            mesh->SetName(name);
            mesh->SetMaterial(material);
            ResourceSaver::SaveToCache(uuidString, mesh);
            return mesh;
        }
    }

    Ref<Mesh> ResourceImporter::ImportMesh(const UUID& uuid)
    {
        std::string uuidString = std::to_string(uuid);

        std::filesystem::path cachedFilePath = CacheManager::GetCachedFilePath(uuidString);

        if(std::filesystem::exists(cachedFilePath))
        {
            const Ref<Resource>& resource = LoadFromCache(cachedFilePath, ResourceFormat::Binary);
            return std::static_pointer_cast<Mesh>(resource);
        }
        else
        {
            COFFEE_WARN("ResourceImporter::ImportMesh: Mesh {0} not found in cache.", (uint64_t)uuid);
            return nullptr;
        }
    }

    Ref<Material> ResourceImporter::ImportMaterial(const std::string& name, const UUID& uuid)
    {
        std::string uuidString = std::to_string(uuid);

        std::filesystem::path cachedFilePath = CacheManager::GetCachedFilePath(uuidString);

        if(std::filesystem::exists(cachedFilePath))
        {
            const Ref<Resource>& resource = LoadFromCache(cachedFilePath, ResourceFormat::Binary);
            return std::static_pointer_cast<Material>(resource);
        }
        else
        {
            COFFEE_WARN("ResourceImporter::ImportMaterial: Material {0} not found in cache. Creating new material.", (uint64_t)uuid);
            Ref<Material> material = CreateRef<Material>(name);
            material->SetUUID(uuid);
            material->SetName(name);
            ResourceSaver::SaveToCache(uuidString, material);
            return material;
        }
    }

    Ref<Material> ResourceImporter::ImportMaterial(const std::string& name, const UUID& uuid, MaterialTextures& materialTextures)
    {
        std::string uuidString = std::to_string(uuid);

        std::filesystem::path cachedFilePath = CacheManager::GetCachedFilePath(uuidString);

        if(std::filesystem::exists(cachedFilePath))
        {
            const Ref<Resource>& resource = LoadFromCache(cachedFilePath, ResourceFormat::Binary);
            return std::static_pointer_cast<Material>(resource);
        }
        else
        {
            COFFEE_WARN("ResourceImporter::ImportMaterial: Material {0} not found in cache. Creating new material.", (uint64_t)uuid);
            Ref<Material> material = CreateRef<Material>(name, materialTextures);
            material->SetUUID(uuid);
            material->SetName(name);
            ResourceSaver::SaveToCache(uuidString, material);
            return material;
        }
    }

    Ref<Material> ResourceImporter::ImportMaterial(const UUID& uuid)
    {
        std::string uuidString = std::to_string(uuid);

        std::filesystem::path cachedFilePath = CacheManager::GetCachedFilePath(uuidString);

        if(std::filesystem::exists(cachedFilePath))
        {
            const Ref<Resource>& resource = LoadFromCache(cachedFilePath, ResourceFormat::Binary);
            return std::static_pointer_cast<Material>(resource);
        }
        else
        {
            COFFEE_WARN("ResourceImporter::ImportMaterial: Material {0} not found in cache.", (uint64_t)uuid);
            return nullptr;
        }
    }

    Ref<Resource> ResourceImporter::LoadFromCache(const std::filesystem::path& path, ResourceFormat format)
        {
            COFFEE_INFO("Loading resource from cache: {0}", path.string());
            switch (format)
            {
                case ResourceFormat::Binary:
                    return BinaryDeserialization(path);
                    break;
                case ResourceFormat::JSON:
                    return JSONDeserialization(path);
                    break;
            }
        }

    Ref<Resource> ResourceImporter::BinaryDeserialization(const std::filesystem::path& path)
    {
        std::ifstream file(path, std::ios::binary);
        cereal::BinaryInputArchive archive(file);
        Ref<Resource> resource;
        archive(resource);
        return resource;
    }

    Ref<Resource> ResourceImporter::JSONDeserialization(const std::filesystem::path& path)
    {
        std::ifstream file(path);
        cereal::JSONInputArchive archive(file);
        Ref<Resource> resource;
        archive(resource);
        return resource;
    }
}