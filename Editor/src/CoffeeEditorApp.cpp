#include <Coffee.h>
#include "CoffeeEngine/Core/EngineContext.h"
#include "CoffeeEngine/Core/EntryPoint.h"
#include "CoffeeEngine/Core/Engine.h"
#include "CoffeeEngine/Core/Application.h"
#include "EditorLayer.h"

// INFO: If I never need to extend the Editor with plugins, do some unit testing or add some debug system is possible to this have a header.

namespace Coffee {

    class CoffeeEditor : public Application
    {
    public:
        EngineContext context;
    public:
        void OnInit(Engine &engine) override
        {
            context = engine.GetContext();
            engine.PushLayer(CreateScope<EditorLayer>());
        }

        void OnUpdate(float dt) override
        {
        }
    };

    Application* CreateApplication()
    {
        return new CoffeeEditor();
    }

}