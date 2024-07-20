#pragma once

#include "TeaEngine/Core/Base.h"
#include "TeaEngine/Renderer/Mesh.h"
#include <assimp/scene.h>
#include <vector>
namespace Tea {

    class Model
    {
    public:
        Model() {};
        Model(const std::string& filePath);

        void LoadModel(const std::string& path);

        const std::vector<Ref<Mesh>>& GetMeshes() const { return m_Meshes; };
        void AddMesh(const Ref<Mesh> mesh) { m_Meshes.push_back(mesh); };
    private:
        Ref<Mesh> processMesh(aiMesh* mesh, const aiScene* scene);
        void processNode(Model* model, aiNode* node, const aiScene* scene);
    private:
        std::vector<Ref<Mesh>> m_Meshes;
        
        std::string m_FilePath;
    };

}