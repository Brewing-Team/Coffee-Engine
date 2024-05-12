#ifndef APPLICATION_H
#define APPLICATION_H

namespace Tea
{

    class Application
    {
      public:
        Application();
        virtual ~Application();

        void Run();
    };

    // To be defined in CLIENT
    Application* CreateApplication();

} // namespace Tea
#endif /* APPLICATION_H */
