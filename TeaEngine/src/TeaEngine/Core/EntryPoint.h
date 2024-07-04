#pragma once

extern Tea::Application* Tea::CreateApplication();

int main(int argc, const char** argv)
{
    Tea::Log::Init();
    TEA_CORE_WARN("Initialized Log!");

    auto app = Tea::CreateApplication();
    app->Run();
    delete app;

    return 0;
}
