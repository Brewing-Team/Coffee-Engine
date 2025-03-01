#include "CoffeeEngine/Renderer/Model.h"
#include "CoffeeEngine/Core/Base.h"
#include "CoffeeEngine/Core/Log.h"
#include "CoffeeEngine/IO/CacheManager.h"
#include "CoffeeEngine/IO/ImportData/MeshImportData.h"
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

        m_Name = m_FilePath.filename().string();

        std::vector<Joint> joints;
        std::map<std::string, int> boneMap;


        if (scene->HasAnimations())
        {
            m_hasAnimations = true;

            if(!ExtractSkeleton(scene, joints, boneMap, *this))
                std::cerr << "Error extracting skeleton" << std::endl;

            if(!ExtractAnimations(scene, boneMap, *this))
                std::cerr << "Error extracting animations" << std::endl;

            if (m_Skeleton && m_AnimationController && m_AnimationController->GetAnimationCount() > 0)
            {
                std::cout << "AnimationsCount: " << m_AnimationController->GetAnimationCount() << ", BonesCount: " << m_Skeleton->GetNumJoints() << std::endl;

                for (const auto& [name, index] : m_AnimationController->GetAnimationMap())
                {
                    std::cout << "AnimationName: " << name << ", Index: " << index << ", Duration: " << m_AnimationController->GetAnimation(index)->GetDuration() << std::endl;
                }
            }
        }

        processNode(scene->mRootNode, scene, joints, boneMap);

        m_Joints = joints;

        if (m_Skeleton)
        {
            m_Skeleton->SetJoints(joints);

            ozz::io::File file("skeleton.ozz", "wb");
            if (file.opened())
            {
                ozz::io::OArchive archive(&file);
                m_Skeleton->Save(archive);
            }
        }

        if (m_AnimationController)
        {
            for (const auto& anim : m_AnimationController->GetAnimations())
            {
                m_AnimationsNames.push_back(anim.GetAnimationName());
                ozz::io::File file((anim.GetAnimationName() + ".ozz").c_str(), "wb");
                if (file.opened())
                {
                    ozz::io::OArchive archive(&file);
                    for (const auto& anim : m_AnimationController->GetAnimations())
                    {
                        anim.Save(archive);
                    }
                }
            }
        }
    }

    Model::Model(ModelImportData& importData)
    {
        if(importData.IsValid())
        {
            s_ModelMeshesUUIDs = importData.meshUUIDs;
            Model(importData.originalPath);
            m_UUID = importData.uuid;
        }
        else
        {
            Model(importData.originalPath);
            importData.uuid = m_UUID;
            importData.meshUUIDs = s_ModelMeshesUUIDs;
        }

        s_ModelMeshesUUIDs.clear();
    }

    Ref<Model> Model::Load(const std::filesystem::path& path)
    {
        return ResourceLoader::Load<Model>(path);
    }

    Ref<Mesh> Model::processMesh(aiMesh* mesh, const aiScene* scene, std::vector<Joint>& joints, std::map<std::string, int>& boneMap)
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

        if (mesh->mNumBones > 0)
        {
            for (unsigned int i = 0; i < mesh->mNumBones; ++i)
            {
                aiBone* bone = mesh->mBones[i];

                int boneID = 0;
                std::string boneName(bone->mName.data);
                if (boneMap.find(boneName) != boneMap.end())
                    boneID = boneMap[boneName];
                else
                {
                    std::cerr << "Found missing joint \"" << boneName << std::endl;
                }

                joints[boneID].invBindPose = AiToGlmMat4(bone->mOffsetMatrix);

                for (unsigned int j = 0; j < bone->mNumWeights; ++j)
                {
                    unsigned int vertexID = bone->mWeights[j].mVertexId;
                    float weight = bone->mWeights[j].mWeight;

                    for (unsigned int g = 0; g < MAX_BONE_INFLUENCE; ++g)
                    {
                        if (vertices[vertexID].BoneWeights[g] == 0.0f)
                        {
                            vertices[vertexID].BoneIDs[g] = boneID;
                            vertices[vertexID].BoneWeights[g] = weight;
                            break;
                        }
                    }
                }
            }
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
    void Model::processNode(aiNode* node, const aiScene* scene, std::vector<Joint>& joints, std::map<std::string, int>& boneMap)
    {
        ZoneScoped;

        m_NodeName = node->mName.C_Str();

        m_Transform = aiMatrix4x4ToGLMMat4(node->mTransformation);

        for(uint32_t i = 0; i < node->mNumMeshes; i++)
        {
            aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
            this->AddMesh(processMesh(mesh, scene, joints, boneMap));
        }

        for(uint32_t i = 0; i < node->mNumChildren; i++)
        {
            Ref<Model> child = CreateRef<Model>();
            child->m_Name = node->mChildren[i]->mName.C_Str();
            child->m_FilePath = m_FilePath;
            child->m_Parent = weak_from_this();
            m_Children.push_back(child);

            child->processNode(node->mChildren[i], scene, joints, boneMap);
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

        Texture2DImportData importData;
        importData.originalPath = texturePath;
        importData.sRGB = srgb;

        return ResourceLoader::Load<Texture2D>(importData);
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

    bool Model::ExtractSkeleton(const aiScene* pScene, std::vector<Joint>& joints, std::map<std::string, int>& boneMap, Model& model)
    {
        ExtractJoints(pScene->mRootNode, -1, joints, boneMap);

        if(joints.empty())
        {
            std::cerr << "Failed to extract joints" << std::endl;
            return false;
        }

        ozz::animation::offline::RawSkeleton rawSkeleton;
        rawSkeleton.roots.resize(1);

        std::function<void(int, ozz::animation::offline::RawSkeleton::Joint&)> buildHierarchy =
            [&](int jointIndex, ozz::animation::offline::RawSkeleton::Joint& outJoint)
            {
                const Joint& joint = joints[jointIndex];
                outJoint.name = joint.name;
                outJoint.transform = joint.localTransform;

                for (size_t i = 0; i < joints.size(); ++i)
                {
                    if (joints[i].parentIndex == jointIndex)
                    {
                        outJoint.children.emplace_back();
                        buildHierarchy(i, outJoint.children.back());
                    }
                }
            };

        for (size_t i = 0; i < joints.size(); ++i)
        {
            if (joints[i].parentIndex == -1)
                buildHierarchy(i, rawSkeleton.roots.back());
        }

        if (!rawSkeleton.Validate())
        {
            std::cerr <<  "Failed to validate Ozz Skeleton" << std::endl;
            return false;
        }

        ozz::animation::offline::SkeletonBuilder skelBuilder;

        auto skeleton = CreateRef<Skeleton>();
        skeleton->SetSkeleton(std::move(skelBuilder(rawSkeleton)));
        skeleton->SetJoints(joints);

        m_Skeleton = skeleton;

        return true;
    }

    bool Model::ExtractAnimations(const aiScene* scene, const std::map<std::string, int>& boneMap, Model& model)
    {
        auto animController = CreateRef<AnimationController>();

        for (unsigned int animIndex = 0; animIndex < scene->mNumAnimations; ++animIndex)
        {
            aiAnimation* aiAnim = scene->mAnimations[animIndex];
            ozz::animation::offline::RawAnimation rawAnimation;
            rawAnimation.duration = static_cast<float>(aiAnim->mDuration / aiAnim->mTicksPerSecond);
            rawAnimation.tracks.resize(boneMap.size());
            rawAnimation.name = aiAnim->mName.C_Str();

            for (unsigned int channelIndex = 0; channelIndex < aiAnim->mNumChannels; ++channelIndex)
            {
                aiNodeAnim* channel = aiAnim->mChannels[channelIndex];
                auto it = boneMap.find(channel->mNodeName.C_Str());

                if (it == boneMap.end())
                    continue;

                int jointIndex = it->second;
                auto& track = rawAnimation.tracks[jointIndex];

                track.translations.reserve(channel->mNumPositionKeys);
                track.rotations.reserve(channel->mNumRotationKeys);
                track.scales.reserve(channel->mNumScalingKeys);

                // Translation
                for (unsigned int i = 0; i < channel->mNumPositionKeys; ++i)
                {
                    const aiVectorKey& key = channel->mPositionKeys[i];
                    track.translations.push_back({
                        static_cast<float>(key.mTime / aiAnim->mTicksPerSecond),
                        ozz::math::Float3(key.mValue.x, key.mValue.y, key.mValue.z)
                    });
                }

                // Rotation
                for (unsigned int i = 0; i < channel->mNumRotationKeys; ++i)
                {
                    const aiQuatKey& key = channel->mRotationKeys[i];
                    track.rotations.push_back({
                        static_cast<float>(key.mTime / aiAnim->mTicksPerSecond),
                        ozz::math::Quaternion(key.mValue.x, key.mValue.y, key.mValue.z, key.mValue.w)
                    });
                }

                // Scale
                for (unsigned int i = 0; i < channel->mNumScalingKeys; ++i)
                {
                    const aiVectorKey& key = channel->mScalingKeys[i];
                    track.scales.push_back({
                        static_cast<float>(key.mTime / aiAnim->mTicksPerSecond),
                        ozz::math::Float3(key.mValue.x, key.mValue.y, key.mValue.z)
                    });
                }
            }

            if (!rawAnimation.Validate())
            {
                std::cerr << "Failed to validate Ozz Animation: " << aiAnim->mName.C_Str() << std::endl;
                continue;
            }

            ozz::animation::offline::AnimationBuilder animBuilder;
            animController->AddAnimation(aiAnim->mName.C_Str(), std::move(animBuilder(rawAnimation)));
        }

        m_AnimationController = animController;

        return true;
    }

    void Model::ExtractJoints(const aiNode* node, int parentIndex, std::vector<Joint>& joints, std::map<std::string, int>& boneMap)
    {
        int jointIndex = 0;
        std::string boneName(node->mName.data);

        if (boneMap.find(boneName) == boneMap.end())
        {
            jointIndex = static_cast<int>(boneMap.size());
            boneMap[boneName] = jointIndex;
        }
        else
            jointIndex = boneMap[boneName];

        joints.push_back({
            .name           = boneName,
            .parentIndex    = parentIndex,
            .localTransform = AiToOzzTransform(node->mTransformation),
            .invBindPose    = glm::mat4(1.0f)
        });

        for (unsigned int i = 0; i < node->mNumChildren; ++i)
            ExtractJoints(node->mChildren[i], jointIndex, joints, boneMap);
    }

}
