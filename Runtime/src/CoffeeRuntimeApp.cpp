#include <Coffee.h>

#include "RuntimeLayer.h"

class CoffeeRuntime : public Coffee::Application
{
  public:
    CoffeeRuntime()
    {
        PushLayer(new Coffee::RuntimeLayer());
    }

    ~CoffeeRuntime()
    {
    }
};

Coffee::Application* Coffee::CreateApplication()
{
    return new CoffeeRuntime();
}