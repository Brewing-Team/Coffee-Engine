#include "RuntimeLayer.h"

#include "CoffeeEngine/Core/Application.h"
#include "CoffeeEngine/Core/Base.h"
#include "CoffeeEngine/Embedded/FinalPassShader.inl"
#include "CoffeeEngine/Events/ApplicationEvent.h"
#include "CoffeeEngine/Project/Project.h"
#include "CoffeeEngine/Renderer/Renderer.h"
#include "CoffeeEngine/Renderer/RendererAPI.h"
#include "CoffeeEngine/Scene/PrimitiveMesh.h"
#include "CoffeeEngine/Renderer/Texture.h"
#include "CoffeeEngine/Scene/Scene.h"
#include "CoffeeEngine/Scene/SceneTree.h"
#include "CoffeeEngine/Scripting/Lua/LuaBackend.h"
#include "CoffeeEngine/Scripting/ScriptManager.h"

#include <cstdint>
#include <filesystem>
#include <glm/fwd.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <imgui.h>
#include <string>
#include <sys/types.h>
#include <tracy/Tracy.hpp>

#include <IconsLucide.h>

namespace Coffee {

    static Ref<Mesh> s_ScreenQuad;
    static Ref<Shader> s_FinalPassShader;

    RuntimeLayer::RuntimeLayer() : Layer("Runtime")
    {

    }

    void RuntimeLayer::OnAttach()
    {
        ZoneScoped;

        s_ScreenQuad = PrimitiveMesh::CreateQuad();
        s_FinalPassShader = CreateRef<Shader>("FinalPassShader", std::string(finalPassShaderSource));

        std::initializer_list<Attachment> ForwardFramebufferAttachments = {
            {ImageFormat::RGBA32F, "Color"},
            {ImageFormat::RGB8, "EntityID"},
            {ImageFormat::DEPTH24STENCIL8, "Depth"}
        };

        std::initializer_list<Attachment> PostProcessingFramebufferAttachments = {
            {ImageFormat::RGBA8, "Color"}
        };

        std::vector<std::pair<std::string, std::initializer_list<Attachment>>> EditorViewportRenderTargetFramebufferAttachments = {
            {"Forward", ForwardFramebufferAttachments},
            {"PostProcessing", PostProcessingFramebufferAttachments}
        };

        m_ViewportRenderTarget = &Renderer::AddRenderTarget("EditorViewport",
                                                            {1600, 900}, 
                                        EditorViewportRenderTargetFramebufferAttachments);

        ScriptManager::RegisterBackend(ScriptingLanguage::Lua, CreateRef<LuaBackend>());

        Project::Load(std::filesystem::current_path() / "gamedata" / "Default.TeaProject");
        Application::Get().GetWindow().SetTitle(Project::GetActive()->GetProjectName());

        // TODO: Load the default scene from the project
        //Project::GetActive().

        m_ActiveScene = Scene::Load(std::filesystem::current_path() / "gamedata" / "Default.TeaScene");

        m_ActiveScene->OnInitRuntime();

        // TODO: Improve this, the event should update the window size
        Renderer::OnResize(1600, 900);
    }

    void RuntimeLayer::OnUpdate(float dt)
    {
        ZoneScoped;

        m_ActiveScene->OnUpdateRuntime(dt);

        Renderer::SetCurrentRenderTarget(nullptr);

        // Render the scene to backbuffer
        const Ref<Texture2D>& finalTexture = Renderer::GetRenderTexture();
        finalTexture->Bind(0);

        s_FinalPassShader->Bind();
        s_FinalPassShader->setInt("screenTexture", 0);

        //RendererAPI::SetClearColor({ 0.1f, 0.1f, 0.1f, 1.0f });
        RendererAPI::Clear();

        RendererAPI::DrawIndexed(s_ScreenQuad->GetVertexArray());

        s_FinalPassShader->Unbind();
    }

    void RuntimeLayer::OnEvent(Coffee::Event& event)
    {
        ZoneScoped;

        m_ActiveScene->OnEvent(event);

        EventDispatcher dispatcher(event);
        dispatcher.Dispatch<WindowResizeEvent>([this](WindowResizeEvent& event) {
            ResizeViewport((float)event.GetWidth(), (float)event.GetHeight());
            return false;
        });
    }

    void RuntimeLayer::OnDetach()
    {
        ZoneScoped;

        m_ActiveScene->OnExitRuntime();
    }

    void RuntimeLayer::ResizeViewport(float width, float height)
    {
        if((m_ViewportSize.x > 0.0f && m_ViewportSize.y > 0.0f) &&
           (width != m_ViewportSize.x || height != m_ViewportSize.y))
        {
            Renderer::OnResize((uint32_t)width, (uint32_t)height);
        }

        m_ViewportSize = { width, height };
    }
}
