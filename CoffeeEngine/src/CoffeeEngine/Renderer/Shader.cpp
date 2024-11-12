#include "CoffeeEngine/Renderer/Shader.h"
#include "CoffeeEngine/IO/ResourceLoader.h"
#include "CoffeeEngine/IO/ResourceRegistry.h"

#include <fstream>
#include <sstream>
#include <iostream>
#include <tracy/Tracy.hpp>

namespace Coffee {

    Shader::Shader(const std::filesystem::path& shaderPath)
    {
        ZoneScoped;

        std::string shaderCode;
        std::ifstream shaderFile;

        shaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);

        try
        {
            shaderFile.open(shaderPath);
            std::stringstream shaderStream;
            shaderStream << shaderFile.rdbuf();
            shaderFile.close();
            shaderCode = shaderStream.str();
        }
        catch (std::ifstream::failure e)
        {
            COFFEE_CORE_ERROR(std::string("ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ: ") + e.what());
        }

        const std::string vertexDelimiter = "#[vertex]";
        const std::string fragmentDelimiter = "#[fragment]";

        size_t vertexPos = shaderCode.find(vertexDelimiter);
        size_t fragmentPos = shaderCode.find(fragmentDelimiter);

        if(vertexPos == std::string::npos || fragmentPos == std::string::npos)
        {
            COFFEE_CORE_ERROR("ERROR::SHADER::DELIMITER_NOT_FOUND: Delimiter not found in shader file!");
            return;
        }

        std::string vertexCode = shaderCode.substr(vertexPos + vertexDelimiter.length(), fragmentPos - vertexPos - vertexDelimiter.length());
        std::string fragmentCode = shaderCode.substr(fragmentPos + fragmentDelimiter.length(), shaderCode.length() - fragmentPos - fragmentDelimiter.length());

        const char* vShaderCode = vertexCode.c_str();
        const char * fShaderCode = fragmentCode.c_str();
        // 2. compile shaders
        unsigned int vertex, fragment;
        // vertex shader
        vertex = glCreateShader(GL_VERTEX_SHADER);
        glShaderSource(vertex, 1, &vShaderCode, NULL);
        glCompileShader(vertex);
        checkCompileErrors(vertex, "VERTEX");
        // fragment Shader
        fragment = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(fragment, 1, &fShaderCode, NULL);
        glCompileShader(fragment);
        checkCompileErrors(fragment, "FRAGMENT");
        // shader Program
        m_ShaderID = glCreateProgram();
        glAttachShader(m_ShaderID, vertex);
        glAttachShader(m_ShaderID, fragment);
        glLinkProgram(m_ShaderID);
        checkCompileErrors(m_ShaderID, "PROGRAM");
        // delete the shaders as they're linked into our program now and no longer necessary
        glDeleteShader(vertex);
        glDeleteShader(fragment);
    }

    Shader::~Shader()
    {
        ZoneScoped;

        glDeleteProgram(m_ShaderID);
    }

    void Shader::Bind()
    {
        ZoneScoped;

        glUseProgram(m_ShaderID);
    }

    void Shader::Unbind()
    {
        ZoneScoped;

        glUseProgram(0);
    }

    void Shader::setBool(const std::string& name, bool value) const
    {
        ZoneScoped;

        GLint location = glGetUniformLocation(m_ShaderID, name.c_str());
        glUniform1i(location, (int)value);
    }

    void Shader::setInt(const std::string& name, int value) const
    {
        ZoneScoped;

        GLint location = glGetUniformLocation(m_ShaderID, name.c_str());
        glUniform1i(location, value);
    }

    void Shader::setFloat(const std::string& name, float value) const
    {
        ZoneScoped;

        GLint location = glGetUniformLocation(m_ShaderID, name.c_str());
        glUniform1f(location, value);
    }

    void Shader::setVec2(const std::string& name, const glm::vec2& value) const
    {
        ZoneScoped;

        GLint location = glGetUniformLocation(m_ShaderID, name.c_str());
        glUniform2fv(location, 1, &value[0]);
    }

    void Shader::setVec3(const std::string& name, const glm::vec3& value) const
    {
        ZoneScoped;

        GLint location = glGetUniformLocation(m_ShaderID, name.c_str());
        glUniform3fv(location, 1, &value[0]);
    }

    void Shader::setVec4(const std::string& name, const glm::vec4& value) const
    {
        ZoneScoped;

        GLint location = glGetUniformLocation(m_ShaderID, name.c_str());
        glUniform4fv(location, 1, &value[0]);
    }

    void Shader::setMat2(const std::string& name, const glm::mat2& mat) const
    {
        ZoneScoped;

        GLint location = glGetUniformLocation(m_ShaderID, name.c_str());
        glUniformMatrix2fv(location, 1, GL_FALSE, &mat[0][0]);
    }

    void Shader::setMat3(const std::string& name, const glm::mat3& mat) const
    {
        ZoneScoped;

        GLint location = glGetUniformLocation(m_ShaderID, name.c_str());
        glUniformMatrix3fv(location, 1, GL_FALSE, &mat[0][0]);
    }

    void Shader::setMat4(const std::string& name, const glm::mat4& mat) const
    {
        ZoneScoped;

        GLint location = glGetUniformLocation(m_ShaderID, name.c_str());
        glUniformMatrix4fv(location, 1, GL_FALSE, &mat[0][0]);
    }

    Ref<Shader> Shader::Create(const std::filesystem::path& shaderPath)
    {
        ZoneScoped;

        return ResourceLoader::LoadShader(shaderPath);
    }

    void Shader::checkCompileErrors(GLuint shader, std::string type)
    {
        GLint success;
        GLchar infoLog[1024];
        if (type != "PROGRAM")
        {
            glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
            if (!success)
            {
                glGetShaderInfoLog(shader, 1024, NULL, infoLog);
                //std::cout << "ERROR::SHADER_COMPILATION_ERROR of type: " << type << "\n" << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
                COFFEE_CORE_ERROR("ERROR::SHADER_COMPILATION_ERROR of type: {0}\n{1}\n", type, infoLog);
            }
        }
        else
        {
            glGetProgramiv(shader, GL_LINK_STATUS, &success);
            if (!success)
            {
                glGetProgramInfoLog(shader, 1024, NULL, infoLog);
                //std::cout << "ERROR::PROGRAM_LINKING_ERROR of type: " << type << "\n" << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
                COFFEE_CORE_ERROR("ERROR::PROGRAM_LINKING_ERROR of type: {0}\n{1}\n", type, infoLog);
            }
        }
    }

}
