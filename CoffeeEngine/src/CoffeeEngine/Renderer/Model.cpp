#include "CoffeeEngine/Renderer/Model.h"
#include "CoffeeEngine/Core/Base.h"
#include "CoffeeEngine/Core/Log.h"
#include "CoffeeEngine/Core/UUID.h"
#include "CoffeeEngine/IO/CacheManager.h"
#include "CoffeeEngine/IO/ImportData/MaterialImportData.h"
#include "CoffeeEngine/IO/ImportData/MeshImportData.h"
#include "CoffeeEngine/IO/ImportData/ModelImportData.h"
#include "CoffeeEngine/IO/ImportData/Texture2DImportData.h"
#include "CoffeeEngine/Renderer/Material.h"
#include "CoffeeEngine/Renderer/Mesh.h"
#include "CoffeeEngine/Renderer/Texture.h"
#include "CoffeeEngine/IO/ResourceLoader.h"

#include <assimp/Importer.hpp>
#include <assimp/material.h>
#include <assimp/mesh.h>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <assimp/types.h>
#include <cstdint>
#include <filesystem>
#include <string>
#include <tracy/Tracy.hpp>
#include <vector>


namespace Coffee {

    glm::mat4 aiMatrix4x4ToGLMMat4(const aiMatrix4x4& aiMat)
    {
        glm::mat4 glmMat;
        glmMat[0][0] = aiMat.a1; glmMat[0][1] = aiMat.b1; glmMat[0][2] = aiMat.c1; glmMat[0][3] = aiMat.d1;
        glmMat[1][0] = aiMat.a2; glmMat[1][1] = aiMat.b2; glmMat[1][2] = aiMat.c2; glmMat[1][3] = aiMat.d2;
        glmMat[2][0] = aiMat.a3; glmMat[2][1] = aiMat.b3; glmMat[2][2] = aiMat.c3; glmMat[2][3] = aiMat.d3;
        glmMat[3][0] = aiMat.a4; glmMat[3][1] = aiMat.b4; glmMat[3][2] = aiMat.c4; glmMat[3][3] = aiMat.d4;
        return glmMat;
    }

    static std::unordered_map<std::string, UUID> s_ModelMeshesUUIDs;
    static std::unordered_map<std::string, UUID> s_ModelMaterialsUUIDs;

    Model::Model(const std::filesystem::path& path)
        : Resource(ResourceType::Model)
    {
        LoadFromFilePath(path);
    }

    Model::Model(ImportData& importData)
        : Resource(ResourceType::Model)
    {
        ModelImportData& modelImportData = dynamic_cast<ModelImportData&>(importData);

        if(modelImportData.IsValid())
        {
            s_ModelMeshesUUIDs = modelImportData.meshUUIDs;
            s_ModelMaterialsUUIDs = modelImportData.materialUUIDs;
            LoadFromFilePath(modelImportData.originalPath);
            m_UUID = modelImportData.uuid;
        }
        else
        {
            LoadFromFilePath(modelImportData.originalPath);
            modelImportData.uuid = m_UUID;
            modelImportData.meshUUIDs = s_ModelMeshesUUIDs;
            modelImportData.materialUUIDs = s_ModelMaterialsUUIDs;
        }

        s_ModelMeshesUUIDs.clear();
        s_ModelMaterialsUUIDs.clear();
    }

    void Model::LoadFromFilePath(const std::filesystem::path& path)
    {
        ZoneScoped;

        m_FilePath = path;

        Assimp::Importer importer;
        const aiScene* scene = importer.ReadFile(m_FilePath.string(), aiProcess_Triangulate | aiProcess_GenSmoothNormals | aiProcess_CalcTangentSpace | aiProcess_GenBoundingBoxes);
        // check for errors
        if(!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) // if is Not Zero
        {
            COFFEE_CORE_ERROR("ERROR::ASSIMP:: {0}", importer.GetErrorString());
            return;
        }

        m_Name = m_FilePath.filename().string();

        processNode(scene->mRootNode, scene);
    }

    Ref<Model> Model::Load(const std::filesystem::path& path)
    {
        return ResourceLoader::Load<Model>(path);
    }

    Ref<Mesh> Model::processMesh(aiMesh* mesh, const aiScene* scene)
    {
        ZoneScoped;

        std::vector<Vertex> vertices;
        std::vector<uint32_t> indices;

        for(uint32_t i = 0; i < mesh->mNumVertices; i++)
        {
            Vertex vertex;
            glm::vec3 vector;

            //positions
            vector.x = mesh->mVertices[i].x;
            vector.y = mesh->mVertices[i].y;
            vector.z = mesh->mVertices[i].z;

            vertex.Position = vector;

            //normals
            if(mesh->HasNormals())
            {
                vector.x = mesh->mNormals[i].x;
                vector.y = mesh->mNormals[i].y;
                vector.z = mesh->mNormals[i].z;

                vertex.Normals = vector;
            }

            //texture coordenates
            if(mesh->mTextureCoords[0]) // does the mesh contain texture coordinates?
            {
                glm::vec2 vec;
                // a vertex can contain up to 8 different texture coordinates. We thus make the assumption that we won't
                // use models where a vertex can have multiple texture coordinates so we always take the first set (0).
                vec.x = mesh->mTextureCoords[0][i].x;
                vec.y = mesh->mTextureCoords[0][i].y;
                vertex.TexCoords = vec;

                // tangent
                vector.x = mesh->mTangents[i].x;
                vector.y = mesh->mTangents[i].y;
                vector.z = mesh->mTangents[i].z;
                vertex.Tangent = vector;
                // bitangent
                vector.x = mesh->mBitangents[i].x;
                vector.y = mesh->mBitangents[i].y;
                vector.z = mesh->mBitangents[i].z;
                vertex.Bitangent = vector;
            }
            else
            {
                vertex.TexCoords = glm::vec2(0.0f, 0.0f);
            }
            vertices.push_back(vertex);
        }
        // now wak through each of the mesh's faces (a face is a mesh its triangle) and retrieve the corresponding vertex indices.

        for(uint32_t i = 0; i < mesh->mNumFaces; i++)
        {
            aiFace face = mesh->mFaces[i];
            for(unsigned int j = 0; j < face.mNumIndices; j++)
                indices.push_back(face.mIndices[j]);
        }

        aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];

        Ref<Material> meshMaterial;

        //The next code is rushed, please Hugo of the future refactor this ;_;
        if(material)
        {
            std::string materialName = (material->GetName().length > 0) ? material->GetName().C_Str() : m_Name;
            std::string referenceName = materialName + "_Mat" + std::to_string(mesh->mMaterialIndex);

            UUID materialUUID;

            if(s_ModelMaterialsUUIDs.find(referenceName) != s_ModelMaterialsUUIDs.end())
            {
                materialUUID = s_ModelMaterialsUUIDs[referenceName];
            }
            else
            {
                materialUUID = UUID();
                s_ModelMaterialsUUIDs[referenceName] = materialUUID;
            }

            MaterialTextures matTextures = LoadMaterialTextures(material);

            MaterialImportData materialImportData;
            materialImportData.name = referenceName;
            materialImportData.materialTextures = &matTextures;
            materialImportData.uuid = materialUUID;
            materialImportData.cachedPath = CacheManager::GetCachedFilePath(materialUUID, ResourceType::Material);
            
            meshMaterial = ResourceLoader::LoadEmbedded<Material>(materialImportData);
        }
        else
        {
            meshMaterial = Material::Create();
        }

        AABB aabb(
            glm::vec3(mesh->mAABB.mMin.x, mesh->mAABB.mMin.y, mesh->mAABB.mMin.z),
            glm::vec3(mesh->mAABB.mMax.x, mesh->mAABB.mMax.y, mesh->mAABB.mMax.z)
            );

        std::string nameReference = m_FilePath.stem().string() + "_" + mesh->mName.C_Str();

        UUID meshUUID;

        if(s_ModelMeshesUUIDs.find(nameReference) != s_ModelMeshesUUIDs.end())
        {
            meshUUID = s_ModelMeshesUUIDs[nameReference];
        }
        else
        {
            meshUUID = UUID();
            s_ModelMeshesUUIDs[nameReference] = meshUUID;
        }

        MeshImportData meshImportData;
        meshImportData.name = nameReference;
        meshImportData.uuid = meshUUID;
        meshImportData.vertices = vertices;
        meshImportData.indices = indices;
        meshImportData.material = meshMaterial;
        meshImportData.aabb = aabb;
        // Think if this is the most comfortable way to do this
        meshImportData.cachedPath = CacheManager::GetCachedFilePath(meshUUID, ResourceType::Mesh);

        Ref<Mesh> resultMesh = ResourceLoader::LoadEmbedded<Mesh>(meshImportData);

        return resultMesh;
    }

    // processes a node in a recursive fashion. Processes each individual mesh located at the node and repeats this process on its children nodes (if any).
    void Model::processNode(aiNode* node, const aiScene* scene)
    {
        ZoneScoped;

        m_NodeName = node->mName.C_Str();

        m_Transform = aiMatrix4x4ToGLMMat4(node->mTransformation);

        for(uint32_t i = 0; i < node->mNumMeshes; i++)
        {
            aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
            this->AddMesh(processMesh(mesh, scene));
        }

        for(uint32_t i = 0; i < node->mNumChildren; i++)
        {
            Ref<Model> child = CreateRef<Model>();
            child->m_Name = node->mChildren[i]->mName.C_Str();
            child->m_FilePath = m_FilePath;
            child->m_Parent = weak_from_this();
            m_Children.push_back(child);

            child->processNode(node->mChildren[i], scene);
        }
    }

    Ref<Texture2D> Model::LoadTexture2D(aiMaterial* material, aiTextureType type)
    {
        aiString textureName;
        material->GetTexture(type, 0, &textureName);

        if(textureName.length == 0)
        {
            return nullptr;
        }

        std::string directory = m_FilePath.parent_path().string();
        std::string texturePath = directory + "/" + std::string(textureName.C_Str());

        bool srgb = (type == aiTextureType_DIFFUSE || type == aiTextureType_EMISSIVE);

        if(!ImportDataUtils::HasImportFile(texturePath))
        {
            Texture2DImportData importData;
            importData.originalPath = texturePath;
            importData.sRGB = srgb;
            importData.uuid = UUID();
            importData.cachedPath = CacheManager::GetCachedFilePath(importData.uuid, ResourceType::Texture2D);
            Scope<ImportData> importDataPtr = CreateScope<Texture2DImportData>(importData);
            ImportDataUtils::SaveImportData(importDataPtr);
            return ResourceLoader::Load<Texture2D>(importData);
        }

        return Texture2D::Load(texturePath);
    }

    MaterialTextures Model::LoadMaterialTextures(aiMaterial* material)
    {
        MaterialTextures matTextures;

        matTextures.albedo = LoadTexture2D(material, aiTextureType_DIFFUSE);
        matTextures.normal = LoadTexture2D(material, aiTextureType_NORMALS);
        matTextures.metallic = LoadTexture2D(material, aiTextureType_METALNESS);
        matTextures.roughness = LoadTexture2D(material, aiTextureType_DIFFUSE_ROUGHNESS);
        matTextures.ao = LoadTexture2D(material, aiTextureType_AMBIENT);

        if(matTextures.ao == nullptr) matTextures.ao = LoadTexture2D(material, aiTextureType_LIGHTMAP);

        matTextures.emissive = LoadTexture2D(material, aiTextureType_EMISSIVE);

        return matTextures;
    }

}
