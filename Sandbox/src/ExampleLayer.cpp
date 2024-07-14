#include "ExampleLayer.h"
#include "TeaEngine/Renderer/Buffer.h"
#include <imgui.h>

#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>

ExampleLayer::ExampleLayer() : Layer("Example")
    {
        float vertices[8 * 3] = {
            // positions        // colors          // Texture Coords
            0.5f, -0.5f, 0.0f,  1.0f, 0.0f, 0.0f,  1.0f, 0.0f,   // bottom right
        -0.5f, -0.5f, 0.0f,  0.0f, 1.0f, 0.0f,  0.0f, 0.0f,   // bottom left
            0.0f,  0.5f, 0.0f,  0.0f, 0.0f, 1.0f,  0.5f, 1.0f    // top
        };

        unsigned int indices[3] = {
            0,1,2
        };

        float cubeVertices[] = {
            // positions          // texture coords
            // Front face
            -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
            0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
            0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
            -0.5f,  0.5f,  0.5f,  0.0f, 1.0f,

            // Back face
            -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
            0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
            0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
            -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,

            // Left face
            -0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
            -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
            -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
            -0.5f, -0.5f,  0.5f,  1.0f, 0.0f,

            // Right face
            0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
            0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
            0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
            0.5f, -0.5f,  0.5f,  1.0f, 0.0f,

            // Top face
            -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
            0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
            0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
            -0.5f,  0.5f,  0.5f,  0.0f, 0.0f,

            // Bottom face
            -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
            0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
            0.5f, -0.5f,  0.5f,  1.0f, 1.0f,
            -0.5f, -0.5f,  0.5f,  0.0f, 1.0f
        };


        unsigned int cubeIndices[] = {
            // Front face
            0, 1, 2,
            2, 3, 0,

            // Back face
            4, 5, 6,
            6, 7, 4,

            // Left face
            8, 9, 10,
            10, 11, 8,

            // Right face
            12, 13, 14,
            14, 15, 12,

            // Top face
            16, 17, 18,
            18, 19, 16,

            // Bottom face
            20, 21, 22,
            22, 23, 20
        };


        m_RendererAPI = Tea::RendererAPI::Create();
        m_RendererAPI->Init();

        m_VertexArray = Tea::VertexArray::Create();
        m_VertexArray->Bind();

        m_VertexBuffer = Tea::VertexBuffer::Create(cubeVertices, sizeof(cubeVertices)); //el size no estoy muy seguro
        Tea::BufferLayout layout = {
            {Tea::ShaderDataType::Vec3, "a_Position"},
            {Tea::ShaderDataType::Vec2, "a_TexCoord"}
        };

        m_VertexBuffer->SetLayout(layout);

        m_IndexBuffer = Tea::IndexBuffer::Create(cubeIndices, 36);

        m_VertexArray->AddVertexBuffer(m_VertexBuffer);
        m_VertexArray->SetIndexBuffer(m_IndexBuffer);
        
        m_defaultShader = Tea::Shader::Create("assets/shaders/TextureShader.vert", "assets/shaders/TextureShader.frag");

        m_Texture = Tea::Texture::Load("assets/textures/test.jpg");

        m_Texture->Bind(0);

        m_defaultShader->Bind();
        m_defaultShader->setInt("tex", 0);

    }

void ExampleLayer::OnUpdate()
{
    //TEA_INFO("ExampleLayer::Update");
    
    m_RendererAPI->SetClearColor({.2f,.2f,.2f,1});
    m_RendererAPI->Clear();

    m_defaultShader->Bind();

    glm::mat4 view = glm::mat4(1.0f);


    view = glm::translate(view, glm::vec3(0.0f, 0.0f, -5.0f));;
    glm::mat4 projection = glm::perspective(glm::radians(45.0f), (float)1600 / (float)900, 0.1f, 100.0f);

    m_defaultShader->setMat4("view", view);
    m_defaultShader->setMat4("projection", projection);

    m_RendererAPI->DrawIndexed(m_VertexArray);
}

void ExampleLayer::OnEvent(Tea::Event& event)
{
    //TEA_TRACE("{0}", event);
}

void ExampleLayer::OnImGuiRender()
{
    ImGui::Begin("debug-cube");

    //ImGui::Text("Texture Parameters")
    ImGui::SeparatorText("Texture Parameters");

    static glm::vec2 textureOffset = glm::vec2(0,0);
    static float scale = 1.0f;

    ImGui::DragFloat2("textureOffset", glm::value_ptr(textureOffset), 0.01);

    ImGui::DragFloat("textureScale", &scale, 0.01);

    m_defaultShader->setVec2("textureOffset", textureOffset);
    m_defaultShader->setFloat("textureScale", scale);

    ImGui::SeparatorText("Transform Parameters");

    static glm::vec3 position = glm::vec3(0.0,0.0,0.0);
    static glm::vec3 dirAxis = glm::vec3(0.0,1.0,0.0);
    static float angle = 0.0;

    ImGui::DragFloat3("position", glm::value_ptr(position), 0.01);
    ImGui::DragFloat3("Axis of rotation", glm::value_ptr(dirAxis), 0.05f, -1.0f, 1.0f);
    ImGui::DragFloat("angle", &angle);

    glm::mat4 model = glm::mat4(1.0f);

    model = glm::translate(model, position);
    model = glm::rotate(model, glm::radians(angle), dirAxis);

    m_defaultShader->setMat4("model", model);

    ImGui::End();
}
