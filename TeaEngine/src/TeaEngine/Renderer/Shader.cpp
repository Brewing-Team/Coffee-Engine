#include "TeaEngine/Renderer/Shader.h"

#include <fstream>
#include <sstream>
#include <iostream>

namespace Tea {

    Shader::Shader(const std::string& vertexPath, const std::string& fragmentPath)
    {
        // 1. retrieve the vertex/fragment source code from filePath
        std::string vertexCode;
        std::string fragmentCode;
        std::ifstream vShaderFile;
        std::ifstream fShaderFile;
        // ensure ifstream objects can throw exceptions:
        vShaderFile.exceptions (std::ifstream::failbit | std::ifstream::badbit);
        fShaderFile.exceptions (std::ifstream::failbit | std::ifstream::badbit);
        try 
        {
            // open files
            vShaderFile.open(vertexPath);
            fShaderFile.open(fragmentPath);
            std::stringstream vShaderStream, fShaderStream;
            // read file's buffer contents into streams
            vShaderStream << vShaderFile.rdbuf();
            fShaderStream << fShaderFile.rdbuf();		
            // close file handlers
            vShaderFile.close();
            fShaderFile.close();
            // convert stream into string
            vertexCode = vShaderStream.str();
            fragmentCode = fShaderStream.str();			
        }
        catch (std::ifstream::failure& e)
        {
            std::cout << "ERROR::SHADER::FILE_NOT_SUCCESSFULLY_READ: " << e.what() << std::endl;
        }
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
        glDeleteProgram(m_ShaderID);
    }

    void Shader::Bind()
    {
        glUseProgram(m_ShaderID);
    }

    void Shader::Unbind()
    {
        glUseProgram(0);
    }

    void Shader::setBool(const std::string& name, bool value) const
    {
        GLint location = glGetUniformLocation(m_ShaderID, name.c_str());
        glUniform1i(location, (int)value);
    }

    void Shader::setInt(const std::string& name, int value) const
    {
        GLint location = glGetUniformLocation(m_ShaderID, name.c_str());
        glUniform1i(location, value);
    }
    
    void Shader::setFloat(const std::string& name, float value) const
    {
        GLint location = glGetUniformLocation(m_ShaderID, name.c_str());
        glUniform1f(location, value);
    }
    
    void Shader::setVec2(const std::string& name, const glm::vec2& value) const
    {
        GLint location = glGetUniformLocation(m_ShaderID, name.c_str());
        glUniform2fv(location, 1, &value[0]);
    }
    
    void Shader::setVec3(const std::string& name, const glm::vec3& value) const
    {
        GLint location = glGetUniformLocation(m_ShaderID, name.c_str());
        glUniform3fv(location, 1, &value[0]);
    }
    
    void Shader::setVec4(const std::string& name, const glm::vec4& value) const
    {
        GLint location = glGetUniformLocation(m_ShaderID, name.c_str());
        glUniform4fv(location, 1, &value[0]);
    }
    
    void Shader::setMat2(const std::string& name, const glm::mat2& mat) const
    {
        GLint location = glGetUniformLocation(m_ShaderID, name.c_str());
        glUniformMatrix2fv(location, 1, GL_FALSE, &mat[0][0]);
    }
    
    void Shader::setMat3(const std::string& name, const glm::mat3& mat) const
    {
        GLint location = glGetUniformLocation(m_ShaderID, name.c_str());
        glUniformMatrix3fv(location, 1, GL_FALSE, &mat[0][0]);
    }
    
    void Shader::setMat4(const std::string& name, const glm::mat4& mat) const
    {
        GLint location = glGetUniformLocation(m_ShaderID, name.c_str());
        glUniformMatrix4fv(location, 1, GL_FALSE, &mat[0][0]);
    }
    
    Ref<Shader> Shader::Create(const std::string& vertexPath, const std::string& fragmentPath)
    {
        return CreateRef<Shader>(vertexPath, fragmentPath);
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
                TEA_CORE_ERROR("ERROR::SHADER_COMPILATION_ERROR of type: {0}\n{1}\n", type, infoLog);
            }
        }
        else
        {
            glGetProgramiv(shader, GL_LINK_STATUS, &success);
            if (!success)
            {
                glGetProgramInfoLog(shader, 1024, NULL, infoLog);
                //std::cout << "ERROR::PROGRAM_LINKING_ERROR of type: " << type << "\n" << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
                TEA_CORE_ERROR("ERROR::PROGRAM_LINKING_ERROR of type: {0}\n{1}\n", type, infoLog);
            }
        }
    }

}