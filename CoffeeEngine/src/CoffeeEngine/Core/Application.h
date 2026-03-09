#pragma once

namespace Coffee
{

class Engine;

class Application
{
public:
    virtual ~Application() = default;

    virtual void OnInit(Engine& engine) {}
    virtual void OnUpdate(float dt) {}
    virtual void OnShutdown() {}
};

Application* CreateApplication();

}