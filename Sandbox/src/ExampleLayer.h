#pragma once

#include <TeaEngine/Renderer/RendererAPI.h>
#include <TeaEngine/Renderer/Shader.h>
#include <TeaEngine/Renderer/Texture.h>
#include <TeaEngine/Core/Layer.h>

class ExampleLayer : public Tea::Layer
{
public:
    ExampleLayer();

    void OnUpdate() override;

    void OnEvent(Tea::Event& event);
private:
    Tea::Ref<Tea::VertexArray> m_VertexArray;
    Tea::Ref<Tea::VertexBuffer> m_VertexBuffer;
    Tea::Ref<Tea::IndexBuffer> m_IndexBuffer;
    Tea::Scope<Tea::RendererAPI> m_RendererAPI;

    Tea::Ref<Tea::Shader> m_defaultShader;
};
