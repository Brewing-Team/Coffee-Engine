#include <Coffee.h>
#include <CoffeeEngine/Core/EntryPoint.h>

#include "EditorLayer.h"

namespace Coffee {

    class CoffeeEditor : public Application
    {
    public:
        CoffeeEditor()
        {
            PushLayer(new EditorLayer());
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