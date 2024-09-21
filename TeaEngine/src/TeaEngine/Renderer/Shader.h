#pragma once

#include "TeaEngine/Core/Base.h"
#include "TeaEngine/IO/Resource.h"

#include <glad/glad.h>
#include <glm/glm.hpp>

#include <string>
#include <unordered_map>

namespace Tea {

    /**
     * @defgroup renderer Renderer
     * @brief Renderer components of the TeaEngine.
     * @{
     */

    /**
     * @brief Class representing a shader program.
     */
    class Shader : public Resource
    {
    public:
        /**
         * @brief Constructs a Shader with the specified vertex and fragment shader paths.
         * @param vertexPath The file path to the vertex shader.
         * @param fragmentPath The file path to the fragment shader.
         */
        Shader(const std::string& vertexPath, const std::string& fragmentPath);

        /**
         * @brief Destructor for the Shader class.
         */
        virtual ~Shader();

        /**
         * @brief Binds the shader program for use.
         */
        void Bind();

        /**
         * @brief Unbinds the shader program.
         */
        void Unbind();

        /**
         * @brief Sets a boolean uniform in the shader.
         * @param name The name of the uniform.
         * @param value The boolean value to set.
         */
        void setBool(const std::string& name, bool value) const;

        /**
         * @brief Sets an integer uniform in the shader.
         * @param name The name of the uniform.
         * @param value The integer value to set.
         */
        void setInt(const std::string& name, int value) const;

        /**
         * @brief Sets a float uniform in the shader.
         * @param name The name of the uniform.
         * @param value The float value to set.
         */
        void setFloat(const std::string& name, float value) const;

        /**
         * @brief Sets a vec2 uniform in the shader.
         * @param name The name of the uniform.
         * @param value The vec2 value to set.
         */
        void setVec2(const std::string& name, const glm::vec2& value) const;

        /**
         * @brief Sets a vec3 uniform in the shader.
         * @param name The name of the uniform.
         * @param value The vec3 value to set.
         */
        void setVec3(const std::string& name, const glm::vec3& value) const;

        /**
         * @brief Sets a vec4 uniform in the shader.
         * @param name The name of the uniform.
         * @param value The vec4 value to set.
         */
        void setVec4(const std::string& name, const glm::vec4& value) const;

        /**
         * @brief Sets a mat2 uniform in the shader.
         * @param name The name of the uniform.
         * @param mat The mat2 value to set.
         */
        void setMat2(const std::string& name, const glm::mat2& mat) const;

        /**
         * @brief Sets a mat3 uniform in the shader.
         * @param name The name of the uniform.
         * @param mat The mat3 value to set.
         */
        void setMat3(const std::string& name, const glm::mat3& mat) const;

        /**
         * @brief Sets a mat4 uniform in the shader.
         * @param name The name of the uniform.
         * @param mat The mat4 value to set.
         */
        void setMat4(const std::string& name, const glm::mat4& mat) const;

        /**
         * @brief Creates a shader from the specified vertex and fragment shader paths.
         * @param vertexPath The file path to the vertex shader.
         * @param fragmentPath The file path to the fragment shader.
         * @return A reference to the created shader.
         */
        static Ref<Shader> Create(const std::string& vertexPath, const std::string& fragmentPath);

        /**
         * @brief Checks for compile errors in the shader.
         * @param shader The shader ID.
         * @param type The type of the shader.
         */
        void checkCompileErrors(GLuint shader, std::string type);

    private:
        unsigned int m_ShaderID; ///< The ID of the shader program.
    };

    /** @} */
}