#include <Tea.h>

#include "ExampleLayer.h"

class Sandbox : public Tea::Application
{
  public:
    Sandbox()
    {
        PushLayer(new ExampleLayer());
    }

    ~Sandbox()
    {
    }
};

Tea::Application* Tea::CreateApplication()
{
    return new Sandbox();
}