#include "ExampleLayer.h"

ExampleLayer::ExampleLayer() : Layer("Example")
    {
        float vertices[3 * 3] = {
            0.5f, -0.5f, 0.0f,
            -0.5f, -0.5f, 0.0f,
            0.0f, 0.5f, 0.0f
        };

        unsigned int indices[3] = {
            0,1,2
        };

        m_RendererAPI = Tea::RendererAPI::Create();
        m_RendererAPI->Init();

        m_VertexArray = Tea::VertexArray::Create();
        m_VertexArray->Bind();

        m_VertexBuffer = Tea::VertexBuffer::Create(vertices, sizeof(vertices)); //el size no estoy muy seguro
        m_IndexBuffer = Tea::IndexBuffer::Create(indices, 3);

        m_VertexArray->AddVertexBuffer(m_VertexBuffer);
        m_VertexArray->SetIndexBuffer(m_IndexBuffer);
        
        m_defaultShader = Tea::Shader::Create("assets/shaders/flatColor.vert", "assets/shaders/flatColor.frag");
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