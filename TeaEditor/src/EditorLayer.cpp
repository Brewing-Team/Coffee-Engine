#include "EditorLayer.h"
#include "TeaEngine/Renderer/RendererAPI.h"

namespace Tea {

    EditorLayer::EditorLayer() : Layer("Example")
    {
      
    }

    void EditorLayer::OnUpdate()
    {
        RendererAPI::SetClearColor({.2f,.2f,.2f,1});
        RendererAPI::Clear();

    }

    void EditorLayer::OnEvent(Tea::Event& event)
    {
    
    }

    void EditorLayer::OnImGuiRender()
    {
    
    }

}
