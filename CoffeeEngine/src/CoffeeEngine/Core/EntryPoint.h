#pragma once

extern Coffee::Application* Coffee::CreateApplication();

int main(int argc, const char** argv)
{
    Coffee::Log::Init();
    COFFEE_CORE_WARN("Initialized Log!");

    Coffee::Engine engine;

    Coffee::Application* app = Coffee::CreateApplication();
    engine.Run(*app);
    delete app;

    return 0;
}
