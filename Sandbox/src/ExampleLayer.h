#pragma once

#include "CoffeeEngine/Renderer/Camera.h"
#include "CoffeeEngine/Renderer/EditorCamera.h"
#include "CoffeeEngine/Renderer/Model.h"
#include <CoffeeEngine/Renderer/RendererAPI.h>
#include <CoffeeEngine/Renderer/Shader.h>
#include <CoffeeEngine/Renderer/Texture.h>
#include <CoffeeEngine/Core/Layer.h>

class ExampleLayer : public Coffee::Layer
{
public:
    ExampleLayer();

    void OnUpdate(float dt) override;

    void OnEvent(Coffee::Event& event);

    void OnImGuiRender();
private:
    Coffee::Ref<Coffee::VertexArray> m_VertexArray;
    Coffee::Ref<Coffee::VertexBuffer> m_VertexBuffer;
    Coffee::Ref<Coffee::IndexBuffer> m_IndexBuffer;
    Coffee::Scope<Coffee::RendererAPI> m_RendererAPI;

    Coffee::Ref<Coffee::Shader> m_defaultShader;
    Coffee::Ref<Coffee::Texture> m_Texture;

    Coffee::Model helmet;
    Coffee::Model plane;

    Coffee::EditorCamera m_EditorCamera;

};
