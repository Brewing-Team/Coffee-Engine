#ifndef ENTRYPOINT_H
#define ENTRYPOINT_H

extern Tea::Application* Tea::CreateApplication();

int main(int argc, const char** argv)
{
    auto app = Tea::CreateApplication();
    app->Run();
    delete app;

    return 0;
}

#endif /* ENTRYPOINT_H */
