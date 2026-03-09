#include <Coffee.h>
#include "CoffeeEngine/Core/EntryPoint.h"
#include "CoffeeEngine/Core/Engine.h"
#include "CoffeeEngine/Core/Application.h"
#include "EditorLayer.h"

namespace Coffee {

    class CoffeeEditor : public Application
    {
    public:
        void OnInit(Engine &engine) override
        {
            engine.PushLayer(new EditorLayer());
        }

        ~CoffeeEditor()
        {
        }
    };

    Application* CreateApplication()
    {
        return new CoffeeEditor();
    }

}