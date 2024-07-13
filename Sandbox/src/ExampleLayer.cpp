#include "ExampleLayer.h"
#include "TeaEngine/Renderer/Buffer.h"
#include <imgui.h>

#include <glm/gtc/type_ptr.hpp>

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

        m_RendererAPI = Tea::RendererAPI::Create();
        m_RendererAPI->Init();

        m_VertexArray = Tea::VertexArray::Create();
        m_VertexArray->Bind();

        m_VertexBuffer = Tea::VertexBuffer::Create(vertices, sizeof(vertices)); //el size no estoy muy seguro
        Tea::BufferLayout layout = {
            {Tea::ShaderDataType::Vec3, "a_Position"},
            {Tea::ShaderDataType::Vec3, "a_Color"},
            {Tea::ShaderDataType::Vec2, "a_TexCoord"}
        };

        m_VertexBuffer->SetLayout(layout);

        m_IndexBuffer = Tea::IndexBuffer::Create(indices, 3);

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
    m_RendererAPI->DrawIndexed(m_VertexArray);
}

void ExampleLayer::OnEvent(Tea::Event& event)
{
    //TEA_TRACE("{0}", event);
}

void ExampleLayer::OnImGuiRender()
{
    ImGui::Begin("debug-texture");

    static glm::vec2 textureOffset = glm::vec2(0,0);
    static float scale = 1.0f;

    ImGui::DragFloat2("textureOffset", glm::value_ptr(textureOffset), 0.01);

    ImGui::DragFloat("textureScale", &scale, 0.01);

    m_defaultShader->setVec2("textureOffset", textureOffset);
    m_defaultShader->setFloat("textureScale", scale);

    ImGui::End();
}
