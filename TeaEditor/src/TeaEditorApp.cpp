#include <Tea.h>
#include <TeaEngine/Core/EntryPoint.h>

#include "EditorLayer.h"

namespace Tea {

    class TeaEditor : public Application
    {
    public:
        TeaEditor()
        {
            PushLayer(new EditorLayer());
        }

        ~TeaEditor()
        {
        }
    };

    Application* CreateApplication()
    {
        return new TeaEditor();
    }

}