#include "TeaEngine/ImGui/ImGuiLayer.h"
#include <Tea.h>

class ExampleLayer : public Tea::Layer
{
public:
    ExampleLayer()
        : Layer("Example")
    {
    }

    void OnUpdate() override
    {
        TEA_INFO("ExampleLayer::Update");
    }

    void OnEvent(Tea::Event& event)
    {
        TEA_TRACE("{0}", event);
    }
};

class Sandbox : public Tea::Application
{
  public:
    Sandbox()
    {
        PushLayer(new ExampleLayer());
        PushOverlay(new Tea::ImGuiLayer());
    }

    ~Sandbox()
    {
    }
};

Tea::Application* Tea::CreateApplication()
{
    return new Sandbox();
}