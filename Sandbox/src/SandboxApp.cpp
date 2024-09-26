#include <Coffee.h>

#include "ExampleLayer.h"

class Sandbox : public Coffee::Application
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

Coffee::Application* Coffee::CreateApplication()
{
    return new Sandbox();
}