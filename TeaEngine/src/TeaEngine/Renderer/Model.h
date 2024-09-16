#pragma once

#include "TeaEngine/Core/Base.h"
#include "TeaEngine/Renderer/Material.h"
#include "TeaEngine/Renderer/Mesh.h"
#include "TeaEngine/Renderer/Texture.h"
#include "TeaEngine/Scene/Scene.h"
#include <assimp/scene.h>
#include <glm/fwd.hpp>
#include <string>
#include <vector>

namespace Tea {

    /**
     * @defgroup renderer Renderer
     * @brief Renderer components of the TeaEngine.
     * @{
     */

    /**
     * @brief Class representing a 3D model.
     */
    class Model
    {
    public:
        /**
         * @brief Default constructor for the Model class.
         */
        Model() {};

        /**
         * @brief Constructs a Model from a file path.
         * @param filePath The file path to the model.
         */
        Model(const std::string& filePath);

        /**
         * @brief Loads a model from the specified path.
         * @param path The path to the model file.
         */
        void LoadModel(const std::string& path);

        /**
         * @brief Gets the meshes of the model.
         * @return A reference to the vector of meshes.
         */
        const std::vector<Ref<Mesh>>& GetMeshes() const { return m_Meshes; };

        /**
         * @brief Adds a mesh to the model.
         * @param mesh A reference to the mesh to add.
         */
        void AddMesh(const Ref<Mesh> mesh) { m_Meshes.push_back(mesh); };

        /**
         * @brief Gets the name of the model.
         * @return A reference to the name of the model.
         */
        std::string& GetName() { return m_Name; }

        /**
         * @brief Gets the parent model.
         * @return A pointer to the parent model.
         */
        const Model* GetParent() const { return m_Parent; }

        /**
         * @brief Gets the children models.
         * @return A reference to the vector of children models.
         */
        const std::vector<Ref<Model>> GetChildren() const { return m_Children; }

        /**
         * @brief Gets the transformation matrix of the model.
         * @return The transformation matrix.
         */
        const glm::mat4 GetTransform() const { return m_Transform; }

    private:
        /**
         * @brief Processes a mesh from the Assimp mesh and scene.
         * @param mesh The Assimp mesh.
         * @param scene The Assimp scene.
         * @return A reference to the processed mesh.
         */
        Ref<Mesh> processMesh(aiMesh* mesh, const aiScene* scene);

        /**
         * @brief Processes a node from the Assimp node and scene.
         * @param node The Assimp node.
         * @param scene The Assimp scene.
         */
        void processNode(aiNode* node, const aiScene* scene);

        /**
         * @brief Loads a texture from the Assimp material and texture type.
         * @param material The Assimp material.
         * @param type The Assimp texture type.
         * @return A reference to the loaded texture.
         */
        Ref<Texture> LoadTexture(aiMaterial* material, aiTextureType type);

        /**
         * @brief Loads material textures from the Assimp material.
         * @param material The Assimp material.
         * @return The loaded material textures.
         */
        MaterialTextures LoadMaterialTextures(aiMaterial* material);

    private:
        std::vector<Ref<Mesh>> m_Meshes; ///< The meshes of the model.

        Model* m_Parent; ///< The parent model.
        std::vector<Ref<Model>> m_Children; ///< The children models.

        std::string m_FilePath; ///< The file path to the model.
        std::string m_Name; ///< The name of the model.

        glm::mat4 m_Transform; ///< The transformation matrix of the model.
    };

    /** @} */
}