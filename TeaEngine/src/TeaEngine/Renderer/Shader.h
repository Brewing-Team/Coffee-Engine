#pragma once

#include "TeaEngine/Core/Base.h"

#include <glad/glad.h>
#include <glm/glm.hpp>

#include <string>

namespace Tea {

    class Shader
    {
    public:
        Shader(const std::string& vertexPath, const std::string& fragmentPath);
        virtual ~Shader();

        void Bind();
        void Unbind();

        void setBool(const std::string& name, bool value) const;
        void setInt(const std::string& name, int value) const;
        void setFloat(const std::string& name, float value) const;
        void setVec2(const std::string& name, const glm::vec2& value) const;
        void setVec3(const std::string& name, const glm::vec3& value) const;
        void setVec4(const std::string& name, const glm::vec4& value) const;
        void setMat2(const std::string& name, const glm::mat2& mat) const;
        void setMat3(const std::string& name, const glm::mat3& mat) const;
        void setMat4(const std::string& name, const glm::mat4& mat) const;

        static Ref<Shader> Create(const std::string& vertexPath, const std::string& fragmentPath);

        void checkCompileErrors(GLuint shader, std::string type);

    private:
        unsigned int m_ShaderID;
    };
}