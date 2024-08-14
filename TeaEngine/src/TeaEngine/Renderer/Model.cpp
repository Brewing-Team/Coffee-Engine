#include "TeaEngine/Renderer/Model.h"
#include "TeaEngine/Core/Base.h"
#include "TeaEngine/Core/Log.h"
#include "TeaEngine/Renderer/Material.h"
#include "TeaEngine/Renderer/Mesh.h"
#include "TeaEngine/Renderer/Texture.h"

#include <assimp/Importer.hpp>
#include <assimp/material.h>
#include <assimp/mesh.h>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <assimp/types.h>
#include <cstdint>
#include <string>
#include <tracy/Tracy.hpp>
#include <vector>


namespace Tea {

    glm::mat4 aiMatrix4x4ToGLMMat4(const aiMatrix4x4& aiMat)
    {
        glm::mat4 glmMat;
        glmMat[0][0] = aiMat.a1; glmMat[0][1] = aiMat.b1; glmMat[0][2] = aiMat.c1; glmMat[0][3] = aiMat.d1;
        glmMat[1][0] = aiMat.a2; glmMat[1][1] = aiMat.b2; glmMat[1][2] = aiMat.c2; glmMat[1][3] = aiMat.d2;
        glmMat[2][0] = aiMat.a3; glmMat[2][1] = aiMat.b3; glmMat[2][2] = aiMat.c3; glmMat[2][3] = aiMat.d3;
        glmMat[3][0] = aiMat.a4; glmMat[3][1] = aiMat.b4; glmMat[3][2] = aiMat.c4; glmMat[3][3] = aiMat.d4;
        return glmMat;
    }

    Model::Model(const std::string& filePath)
    {
        ZoneScoped;

        m_FilePath = filePath;

        LoadModel(m_FilePath);
    }

    void Model::LoadModel(const std::string& path)
    {
        Assimp::Importer importer;
        const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_GenSmoothNormals | aiProcess_FlipUVs | aiProcess_CalcTangentSpace);
        // check for errors
        if(!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) // if is Not Zero
        {
            TEA_CORE_ERROR("ERROR::ASSIMP:: {0}", importer.GetErrorString());
            return;
        }

        m_Name = scene->mRootNode->mName.C_Str();

        processNode(scene->mRootNode, scene);
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
                vec.y = 1 - mesh->mTextureCoords[0][i].y;
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
        
        std::string directory = m_FilePath.substr(0, m_FilePath.find_last_of('/') + 1);

        aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];

        Ref<Material> meshMaterial;


        //The next code is rushed, please Hugo of the future refactor this ;_;
        if(material)
        {
            //This needs an urgent optimization, I need the Libraries of this Resources for reuse this Textures and Materials.
            aiString textureName;
            material->GetTexture(aiTextureType_DIFFUSE, 0, &textureName);
            std::string texturePath = directory + std::string(textureName.C_Str());
            if(texturePath != directory)
            {
                MaterialTextures matTextures;
                matTextures.albedo = Texture::Load(texturePath);
                meshMaterial = CreateRef<Material>(matTextures);
            }
            else
            {
                meshMaterial = CreateRef<Material>();
            }
        }
        else
        {
            meshMaterial = CreateRef<Material>();
        }

        Ref<Mesh> resultMesh = CreateRef<Mesh>(indices, vertices);
        resultMesh->SetName(mesh->mName.C_Str());
        resultMesh->SetMaterial(meshMaterial);

        return resultMesh;
    }

    // processes a node in a recursive fashion. Processes each individual mesh located at the node and repeats this process on its children nodes (if any).
    void Model::processNode(aiNode* node, const aiScene* scene)
    {
        ZoneScoped;

        m_Name = node->mName.C_Str();

        m_Transform = aiMatrix4x4ToGLMMat4(node->mTransformation);

        for(uint32_t i = 0; i < node->mNumMeshes; i++)
        {
            aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
            this->AddMesh(processMesh(mesh, scene));
        }

        for(uint32_t i = 0; i < node->mNumChildren; i++)
        {
            Ref<Model> child = CreateRef<Model>();
            child->m_FilePath = m_FilePath;
            child->m_Parent = this;
            m_Children.push_back(child);

            child->processNode(node->mChildren[i], scene);
        }
    }

}
