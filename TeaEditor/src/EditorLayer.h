#pragma once

#include <TeaEngine/Core/Layer.h>

namespace Tea {

    class EditorLayer : public Tea::Layer
    {
    public:
        EditorLayer();
        virtual ~EditorLayer() = default;

        void OnUpdate() override;

        void OnEvent(Tea::Event& event);

        void OnImGuiRender();
    private:

    };
    
}
