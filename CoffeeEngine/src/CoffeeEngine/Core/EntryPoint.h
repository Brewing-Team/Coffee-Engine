#pragma once

extern Coffee::Application* Coffee::CreateApplication();

int main(int argc, const char** argv)
{
    Coffee::Log::Init();
    COFFEE_CORE_WARN("Initialized Log!");

    auto app = Coffee::CreateApplication();
    app->Run();
    delete app;

    return 0;
}
