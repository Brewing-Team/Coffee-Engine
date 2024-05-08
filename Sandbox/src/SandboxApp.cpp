#include <Tea.h>

class Sandbox : public Tea::Application
{
  public:
    Sandbox()
    {
    }

    ~Sandbox()
    {
    }
};

Tea::Application* Tea::CreateApplication()
{
    return new Sandbox();
}