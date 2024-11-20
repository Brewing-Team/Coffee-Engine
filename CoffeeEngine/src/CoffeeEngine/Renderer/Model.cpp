#include "CoffeeEngine/Renderer/Model.h"
#include "CoffeeEngine/Core/Base.h"
#include "CoffeeEngine/Core/Log.h"
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

    Model::Model(const std::filesystem::path& path)
        : Resource(ResourceType::Model)
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

        m_Name = m_FilePath.filename();

        processNode(scene->mRootNode, scene);
    }

    Ref<Model> Model::Load(const std::filesystem::path& path)
    {
        return ResourceLoader::LoadModel(path, true);
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
            MaterialTextures matTextures = LoadMaterialTextures(material);
            std::string materialName = (material->GetName().length > 0) ? material->GetName().C_Str() : m_Name;
            std::string referenceName = materialName + "_Mat" + std::to_string(mesh->mMaterialIndex);
            meshMaterial = Material::Create(referenceName, &matTextures);
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
        Ref<Mesh> resultMesh = ResourceLoader::LoadMesh(nameReference, vertices, indices);
        //resultMesh->SetName(mesh->mName.C_Str());
        //TODO: When the UUID is implemented, the name of the mesh will be resultMesh->SetName(mesh->mName.C_Str());, are your sure?
        //resultMesh->SetName(nameReference);
        resultMesh->SetMaterial(meshMaterial);
        resultMesh->SetAABB(aabb);

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

    Ref<Texture> Model::LoadTexture(aiMaterial* material, aiTextureType type)
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

        return Texture::Load(texturePath, srgb);
    }

    MaterialTextures Model::LoadMaterialTextures(aiMaterial* material)
    {
        MaterialTextures matTextures;

        matTextures.albedo = LoadTexture(material, aiTextureType_DIFFUSE);
        matTextures.normal = LoadTexture(material, aiTextureType_NORMALS);
        matTextures.metallic = LoadTexture(material, aiTextureType_METALNESS);
        matTextures.roughness = LoadTexture(material, aiTextureType_DIFFUSE_ROUGHNESS);
        matTextures.ao = LoadTexture(material, aiTextureType_AMBIENT);

        if(matTextures.ao == nullptr) matTextures.ao = LoadTexture(material, aiTextureType_LIGHTMAP);

        matTextures.emissive = LoadTexture(material, aiTextureType_EMISSIVE);

        return matTextures;
    }

}
