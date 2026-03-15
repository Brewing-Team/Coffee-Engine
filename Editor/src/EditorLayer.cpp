#include "EditorLayer.h"

#include "CoffeeEngine/Core/Assert.h"
#include "CoffeeEngine/Core/Base.h"
#include "CoffeeEngine/Core/FileDialog.h"
#include "CoffeeEngine/Core/Input.h"
#include "CoffeeEngine/Core/Log.h"
#include "CoffeeEngine/Core/MouseCodes.h"
#include "CoffeeEngine/Core/Window.h"
#include "CoffeeEngine/ImGui/ImGuiLayer.h"
#include "CoffeeEngine/Events/ApplicationEvent.h"
#include "CoffeeEngine/Events/KeyEvent.h"
#include "CoffeeEngine/Project/Project.h"
#include "CoffeeEngine/Project/ProjectManager.h"
#include "CoffeeEngine/Resources/ResourceManager.h"
#include "CoffeeEngine/Resources/ResourceUtils.h"
#include "CoffeeEngine/Rendering/EditorCamera.h"
#include "CoffeeEngine/Rendering/Framebuffer.h"
#include "CoffeeEngine/Rendering/Material.h"
#include "CoffeeEngine/Rendering/Mesh.h"
#include "CoffeeEngine/Rendering/RenderTarget.h"
#include "CoffeeEngine/Rendering/Renderer.h"
#include "CoffeeEngine/Rendering/Renderer2D.h"
#include "CoffeeEngine/Rendering/Renderer3D.h"
#include "CoffeeEngine/Rendering/Texture.h"
#include "CoffeeEngine/Rendering/PrimitiveMesh.h"
#include "CoffeeEngine/Scene/Scene.h"
#include "CoffeeEngine/Scene/SceneCamera.h"
#include "CoffeeEngine/Scene/SceneTree.h"
#include "CoffeeEngine/Scripting/Lua/LuaBackend.h"
#include "CoffeeEngine/Scripting/ScriptingManager.h"
#include "Panels/SceneTreePanel.h"
#include "entt/entity/entity.hpp"
#include "imgui_internal.h"
#include "CoffeeEngine/Scene/SceneManager.h"

#include <ImGuizmo.h>
#include <SDL3/SDL_events.h>
#include <stdint.h>
#include <filesystem>
#include <glm/fwd.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <imgui.h>
#include <initializer_list>
#include <string>
#include <sys/types.h>
#include <tracy/Tracy.hpp>

#include <IconsLucide.h>
#include <utility>
#include <vector>

namespace Coffee {

    EditorLayer::EditorLayer(const EngineContext& ctx) : Layer("Example"), m_Context(ctx)
    {

    }

    void EditorLayer::OnAttach()
    {
        ZoneScoped;

        // Create texture from texture parameters
        // Add it to the framebuffers
        // Create the RenderTarget and set it to the Renderer
        TextureProperties textureProperties;
        textureProperties.Width = 1280;
        textureProperties.Height = 720;
        textureProperties.Format = ImageFormat::RGBA32F;
        textureProperties.srgb = false;
        textureProperties.GenerateMipmaps = false;
        textureProperties.Wrapping = TextureWrap::ClampToEdge;
        textureProperties.MinFilter = TextureFilter::Linear;
        textureProperties.MagFilter = TextureFilter::Linear;

        Ref<Texture2D> forwardColorTexture = Texture2D::Create(textureProperties);


        textureProperties.Format = ImageFormat::RGB8;
        Ref<Texture2D> forwardEntityIDTexture = Texture2D::Create(textureProperties);

        textureProperties.Format = ImageFormat::DEPTH24STENCIL8;
        Ref<Texture2D> forwardDepthTexture = Texture2D::Create(textureProperties);

        Ref<Framebuffer> forwardFramebuffer = Framebuffer::Create(1280, 720);
        forwardFramebuffer->AttachColorTexture(0, forwardColorTexture);
        forwardFramebuffer->AttachColorTexture(1, forwardEntityIDTexture);
        forwardFramebuffer->AttachDepthTexture(forwardDepthTexture);

        textureProperties.Format = ImageFormat::RGBA32F;
        Ref<Texture2D> postProcessingColorTextureA = Texture2D::Create(textureProperties);
        Ref<Texture2D> postProcessingColorTextureB = Texture2D::Create(textureProperties);

        Ref<Framebuffer> postProcessingFramebufferA = Framebuffer::Create(1280, 720);
        postProcessingFramebufferA->AttachColorTexture(0, postProcessingColorTextureA);

        Ref<Framebuffer> postProcessingFramebufferB = Framebuffer::Create(1280, 720);
        postProcessingFramebufferB->AttachColorTexture(0, postProcessingColorTextureB);

        m_ViewportRenderTarget = CreateRef<RenderTarget>("EditorViewport", glm::vec2(1280, 720));
        m_ViewportRenderTarget->AddFramebuffer("Forward", forwardFramebuffer);
        m_ViewportRenderTarget->AddFramebuffer("PostProcessingA", postProcessingFramebufferA);
        m_ViewportRenderTarget->AddFramebuffer("PostProcessingB", postProcessingFramebufferB);

        m_Context.renderer->AddRenderTarget(m_ViewportRenderTarget);

        m_Context.scripting->RegisterBackend(ScriptingLanguage::Lua, CreateRef<LuaBackend>());

        m_Context.window->SetIcon("icon.png");

        m_EditorScene = CreateRef<Scene>(const_cast<EngineContext&>(m_Context));
            
        m_Context.sceneManager->SetSceneState(SceneManager::SceneState::Edit);
        m_Context.sceneManager->ChangeScene(m_EditorScene);

        m_EditorCamera = EditorCamera(m_Context.input, 45.0f);

        m_SceneTreePanel.SetContext(m_Context.sceneManager->GetActiveScene());
        m_ContentBrowserPanel.SetContext(m_Context.sceneManager->GetActiveScene());
        m_ContentBrowserPanel.SetEngineContext(m_Context);
        m_ImportPanel.SetEngineContext(m_Context);
        m_ProjectSettingsPanel.SetContext(m_Context);
    }

    void EditorLayer::OnUpdate(float dt)
    {
        ZoneScoped;
        
        // Idk if this is the best place or is better in each switch case for flexibility
        // Is possible that this is does not what I think it does. It should be revised.
        m_Context.renderer->SetCurrentRenderTarget(m_ViewportRenderTarget.get());

        switch (m_Context.sceneManager->GetSceneState())
        {
            using enum SceneManager::SceneState;

            case Edit:
                m_EditorCamera.OnUpdate(dt);
                m_Context.sceneManager->GetActiveScene()->OnUpdateEditor(m_EditorCamera, dt);
                OnOverlayRender();
            break;
            case Play:
                m_Context.sceneManager->GetActiveScene()->OnUpdateRuntime(dt);
            break;

        }
        
        m_Context.renderer->SetCurrentRenderTarget(nullptr);
    }

    void EditorLayer::OnEvent(Coffee::Event& event)
    {
        ZoneScoped;

        m_EditorCamera.OnEvent(event);

        m_Context.sceneManager->GetActiveScene()->OnEvent(event);

        EventDispatcher dispatcher(event);
        dispatcher.Dispatch<KeyPressedEvent>(COFFEE_BIND_EVENT_FN(EditorLayer::OnKeyPressed));
        dispatcher.Dispatch<MouseButtonPressedEvent>(COFFEE_BIND_EVENT_FN(EditorLayer::OnMouseButtonPressed));
        dispatcher.Dispatch<FileDropEvent>(COFFEE_BIND_EVENT_FN(EditorLayer::OnFileDrop));
    }

    bool EditorLayer::OnKeyPressed(KeyPressedEvent& event)
    {
        if(event.IsRepeat() > 0)
            return false;

        switch (event.GetKeyCode())
        {
            case Coffee::Key::G:
                m_GizmoType = ImGuizmo::OPERATION::TRANSLATE;
            break;
            case Coffee::Key::R:
                m_GizmoType = ImGuizmo::OPERATION::ROTATE;
            break;
            case Coffee::Key::S:
                m_GizmoType = ImGuizmo::OPERATION::SCALE;
            break;
            case Coffee::Key::F:
                if(m_SceneTreePanel.GetSelectedEntity())
                {
                    glm::mat4 worldTransform = m_SceneTreePanel.GetSelectedEntity().GetComponent<TransformComponent>().GetWorldTransform();

                    m_EditorCamera.SetFocusPoint(glm::vec3(worldTransform[3]));
                }
            break;
        }

        return false;
    }

    bool EditorLayer::OnMouseButtonPressed(MouseButtonPressedEvent& event)
    {
        if (event.GetMouseButton() == Mouse::ButtonLeft)
        {
            if (m_ViewportHovered && !ImGuizmo::IsOver() && !ImGuizmo::IsUsing())
            {
                //TODO: Clean this up and wrap it in a function
                glm::vec2 mousePos = m_Context.input->GetMousePosition();
                mousePos.x -= m_ViewportBounds[0].x;
                mousePos.y -= m_ViewportBounds[0].y;
                glm::vec2 viewportSize = m_ViewportBounds[1] - m_ViewportBounds[0];
                mousePos.y = viewportSize.y - mousePos.y;
                int mouseX = (int)mousePos.x;
                int mouseY = (int)mousePos.y;

                if (mouseX >= 0 && mouseY >= 0 && mouseX < (int)viewportSize.x && mouseY < (int)viewportSize.y)
                {
                    const glm::vec4& pixelData = m_ViewportRenderTarget->GetFramebuffer("Forward")->GetPixelColor(mouseX, mouseY, 1);

                    /// Convert the vec3 back to uint32_t
                    uint32_t r = static_cast<uint32_t>(pixelData.r * 255.0f);
                    uint32_t g = static_cast<uint32_t>(pixelData.g * 255.0f);
                    uint32_t b = static_cast<uint32_t>(pixelData.b * 255.0f);

                    uint32_t entityID = (r << 0) | (g << 8) | (b << 16);

                    Entity hoveredEntity = entityID == 16777215 ? Entity() : Entity((entt::entity)entityID, m_Context.sceneManager->GetActiveScene().get());

                    m_SceneTreePanel.SetSelectedEntity(hoveredEntity);
                }
            }
        }
        return false;
    }

    bool EditorLayer::OnFileDrop(FileDropEvent& event)
    {
        // Take the file path from the event and check if it is a folder or a file
        //Then copy it to the project folder
        const std::filesystem::path& projectDirectory = m_Context.projectManager->GetCurrentProject()->GetDirectory();
        const std::filesystem::path& originPath = event.GetFile();
        const std::filesystem::path& destFilePath = projectDirectory / originPath.filename();
        std::filesystem::copy(originPath, destFilePath, std::filesystem::copy_options::recursive);
        
        if(std::filesystem::is_directory(destFilePath))
        {
            m_Context.resourceManager->LoadDirectory(destFilePath);
        }
        else
        {
            m_Context.resourceManager->LoadFile(destFilePath);
        }
        return false;
    }

    void EditorLayer::OnDetach()
    {
        ZoneScoped;

        m_Context.sceneManager->GetActiveScene()->OnExitEditor();
    }

    void EditorLayer::OnImGuiRender()
    {
        ZoneScoped;

        ImGui::DockSpaceOverViewport(0, ImGui::GetMainViewport());

        struct MainMenuWindows
        {
            bool EditorSettings = false;
            bool ProjectSettings = false;
            bool AboutCoffeeEngine = false;
        }static mainMenuWindows;

        std::string mainMenuAction = "";
        if (ImGui::BeginMainMenuBar()) {

            if (ImGui::BeginMenu("File"))
            {
                if (ImGui::MenuItem(ICON_LC_FILE_PLUS_2 " New Scene", "Ctrl+N")) { NewScene(); }
                if (ImGui::MenuItem(ICON_LC_FOLDER_OPEN " Open Scene...", "Ctrl+O")) { OpenScene(); }
                if (ImGui::MenuItem(ICON_LC_SAVE " Save Scene", "Ctrl+S")) { SaveScene(); }
                //if (ImGui::MenuItem(ICON_LC_SAVE " Save Scene As...", "Ctrl+Shift+S")) { SaveSceneAs(); }
                if (ImGui::MenuItem(ICON_LC_X " Exit")) {
                    SDL_Event quitEvent{};
                    quitEvent.type = SDL_EVENT_QUIT;
                    SDL_PushEvent(&quitEvent);
                }
                ImGui::EndMenu();
            }
            if (ImGui::BeginMenu("Project"))
            {
                if (ImGui::MenuItem(ICON_LC_FILE_PLUS_2 " New Project...", "Ctrl+N")) { NewProject(); }
                if (ImGui::MenuItem(ICON_LC_FOLDER_OPEN " Open Project...", "Ctrl+O")) { OpenProject(); }
                if (ImGui::MenuItem(ICON_LC_SAVE " Save Project", "Ctrl+S")) { SaveProject(); }
                if (ImGui::MenuItem(ICON_LC_SETTINGS " Project Settings", nullptr, mainMenuWindows.ProjectSettings))
                {
                    mainMenuWindows.ProjectSettings = !mainMenuWindows.ProjectSettings;
                }
                ImGui::EndMenu();
            }
            if (ImGui::BeginMenu("Editor"))
            {
                if(ImGui::BeginMenu("Color Theme"))
                {
                    if(ImGui::MenuItem(ICON_LC_COFFEE " Coffee"))
                    {
                        // Theme switching is managed by the host app layer.
                    }
                    if(ImGui::MenuItem("Godot"))
                    {
                        // Theme switching is managed by the host app layer.
                    }
                    if(ImGui::MenuItem("Tea"))
                    {
                        // Theme switching is managed by the host app layer.
                    }
                    ImGui::EndMenu();
                }
                if(ImGui::BeginMenu("Windows"))
                {
                    if(ImGui::MenuItem("Scene Tree", nullptr, m_SceneTreePanel.IsVisible())) { m_SceneTreePanel.ToggleVisibility(); }
                    if(ImGui::MenuItem("Content Browser", nullptr, m_ContentBrowserPanel.IsVisible())) { m_ContentBrowserPanel.ToggleVisibility(); }
                    if(ImGui::MenuItem("Output", nullptr, m_OutputPanel.IsVisible())) { m_OutputPanel.ToggleVisibility(); }
                    ImGui::EndMenu();
                }
                if(ImGui::MenuItem("Editor Settings"))
                {
                    mainMenuWindows.EditorSettings = true;
                }
                ImGui::EndMenu();
            }
            if (ImGui::BeginMenu("Debug"))
            {
                Ref<Scene> activeScene = m_Context.sceneManager->GetActiveScene();
                bool isSceneActive = activeScene != nullptr;

                if (!isSceneActive)
                    ImGui::BeginDisabled();

                if (ImGui::MenuItem("Debug Draw", nullptr, isSceneActive ? activeScene->GetDebugFlags().DebugDraw : false))
                {
                    if (isSceneActive)
                    {
                        // Toggle the debug draw flag
                        bool newState = !activeScene->GetDebugFlags().DebugDraw;
                        activeScene->GetDebugFlags().DebugDraw = newState;
                        
                        // Set all other debug flags to match the main debug draw flag
                        activeScene->GetDebugFlags().ShowOctree = newState;
                        activeScene->GetDebugFlags().ShowColliders = newState;
                        activeScene->GetDebugFlags().ShowNavMesh = newState;
                        activeScene->GetDebugFlags().ShowNavMeshPath = newState;
                    }
                }

                if (ImGui::MenuItem("Show Octree", nullptr, isSceneActive ? activeScene->GetDebugFlags().ShowOctree : false))
                    if (isSceneActive)
                        activeScene->GetDebugFlags().ShowOctree = !activeScene->GetDebugFlags().ShowOctree;

                if (ImGui::MenuItem("Show Colliders", nullptr, isSceneActive ? activeScene->GetDebugFlags().ShowColliders : false))
                    if (isSceneActive)
                        activeScene->GetDebugFlags().ShowColliders = !activeScene->GetDebugFlags().ShowColliders;

                if (ImGui::MenuItem("Show NavMesh", nullptr, isSceneActive ? activeScene->GetDebugFlags().ShowNavMesh : false))
                    if (isSceneActive)
                        activeScene->GetDebugFlags().ShowNavMesh = !activeScene->GetDebugFlags().ShowNavMesh;

                if (ImGui::MenuItem("Show NavMeshPath", nullptr, isSceneActive ? activeScene->GetDebugFlags().ShowNavMeshPath : false))
                    if (isSceneActive)
                        activeScene->GetDebugFlags().ShowNavMeshPath = !activeScene->GetDebugFlags().ShowNavMeshPath;

                if (!isSceneActive)
                    ImGui::EndDisabled();

                ImGui::EndMenu();
            }

            if (ImGui::BeginMenu("About"))
            {
                if(ImGui::MenuItem("About Coffee Engine"))
                {
                    mainMenuAction = "About Coffee Engine";
                }
                ImGui::EndMenu();
            }

            //Play and Stop buttons
            ImGui::SetCursorPosX(ImGui::GetWindowWidth() * 0.5f - 50);

            switch (m_Context.sceneManager->GetSceneState())
            {
                using enum SceneManager::SceneState;

                case Edit:
                    if(ImGui::Button(ICON_LC_PLAY))
                    {
                        OnScenePlay();
                    }
                break;
                case Play:
                    if(ImGui::Button(ICON_LC_SQUARE))
                    {
                        OnSceneStop();
                    }
                break;
            }

            //set the fps counter in the right side of the menu bar
            const float fps = ImGui::GetIO().Framerate;
            ImVec2 textSize = ImGui::CalcTextSize(("FPS:" + std::to_string(fps)).c_str());
            ImGui::SetCursorPosX(ImGui::GetWindowWidth() - textSize.x);
            ImGui::TextDisabled("FPS: %.1f", fps);

            ImGui::EndMainMenuBar();
        }

        if(mainMenuWindows.ProjectSettings)
        {
            m_ProjectSettingsPanel.OnImGuiRender();
        }

        // Editor Settings Popup

        // About Coffee Engine Popup
        if(mainMenuAction == "About Coffee Engine"){ ImGui::OpenPopup("About Coffee Engine"); }
        ImGui::SetNextWindowPos(ImVec2(ImGui::GetIO().DisplaySize.x * 0.5f, ImGui::GetIO().DisplaySize.y * 0.5f), ImGuiCond_Always, ImVec2(0.5f, 0.5f));
        ImGui::SetNextWindowSize({400, 150});
        if (ImGui::BeginPopupModal("About Coffee Engine", NULL, ImGuiWindowFlags_AlwaysAutoResize))
        {
            ImGui::TextWrapped("Coffee Engine is a 3D Game Engine developed by the Brewing Team.");
            ImGui::TextWrapped("This project is open source and can be found at:");
            ImGui::TextLinkOpenURL("https://github.com/Brewing-Team/Coffee-Engine");
            ImGui::SetCursorPosX(ImGui::GetContentRegionAvail().x / 2 - ImGui::CalcTextSize("Close").x / 2);
            if (ImGui::Button("Close"))
            {
                ImGui::CloseCurrentPopup();
            }
            ImGui::EndPopup();
        }

        // Render the panels
        m_SceneTreePanel.OnImGuiRender();
        m_ContentBrowserPanel.OnImGuiRender();
        m_OutputPanel.OnImGuiRender();
        m_MonitorPanel.OnImGuiRender();

        m_ImportPanel.SetSelectedResource(m_ContentBrowserPanel.GetSelectedResource());
        m_ImportPanel.OnImGuiRender();

        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));
        ImGui::Begin("Viewport");
        auto viewportMinRegion = ImGui::GetWindowContentRegionMin();
		auto viewportMaxRegion = ImGui::GetWindowContentRegionMax();
		auto viewportOffset = ImGui::GetWindowPos();
		m_ViewportBounds[0] = { viewportMinRegion.x + viewportOffset.x, viewportMinRegion.y + viewportOffset.y };
		m_ViewportBounds[1] = { viewportMaxRegion.x + viewportOffset.x, viewportMaxRegion.y + viewportOffset.y };

        m_ViewportFocused = ImGui::IsWindowFocused();
        m_ViewportHovered = ImGui::IsWindowHovered();
        if (m_Context.imguiLayer)
            m_Context.imguiLayer->BlockEvents(!(m_ViewportFocused && m_ViewportHovered));

        ImVec2 viewportPanelSize = ImGui::GetContentRegionAvail();
        ResizeViewport(viewportPanelSize.x, viewportPanelSize.y);

        uint32_t textureID = m_ViewportRenderTarget->GetFramebuffer("Forward")->GetColorAttachment(0)->GetID();
        ImGui::Image((ImTextureID)(uintptr_t)textureID, ImVec2{ m_ViewportSize.x, m_ViewportSize.y }, {0, 1}, {1, 0});

        //Guizmo
        Entity selectedEntity = m_SceneTreePanel.GetSelectedEntity();

        if(selectedEntity and m_GizmoType != -1 and m_Context.sceneManager->GetSceneState() == SceneManager::SceneState::Edit)
        {
            if (selectedEntity.HasComponent<UIImageComponent>())
            {
                auto& transformComponent = selectedEntity.GetComponent<TransformComponent>();

                ImGuizmo::SetOrthographic(true);
                ImGuizmo::SetDrawlist();
                ImGuizmo::SetRect(m_ViewportBounds[0].x, m_ViewportBounds[0].y, m_ViewportBounds[1].x - m_ViewportBounds[0].x, m_ViewportBounds[1].y - m_ViewportBounds[0].y);

                glm::mat4 transform = transformComponent.GetWorldTransform();

                glm::mat4 cameraProjection = glm::ortho(-m_ViewportSize.x / 2.0f, m_ViewportSize.x / 2.0f,
                                        m_ViewportSize.y / 2.0f, -m_ViewportSize.y / 2.0f,
                                        -1.0f, 1.0f);

                glm::mat4 cameraView = glm::mat4(1.0f);

                ImGuizmo::SetGizmoSizeClipSpace(0.15f);

                if (m_GizmoType == ImGuizmo::OPERATION::ROTATE)
                    m_GizmoType = ImGuizmo::OPERATION::ROTATE_Z;

                ImGuizmo::Manipulate(glm::value_ptr(cameraView), glm::value_ptr(cameraProjection),
                     (ImGuizmo::OPERATION)m_GizmoType, ImGuizmo::LOCAL,
                     glm::value_ptr(transform));

                if (ImGuizmo::IsUsing())
                {
                    glm::vec3 translation, scale;
                    glm::quat rotation;
                    ImGuizmo::DecomposeMatrixToComponents(glm::value_ptr(transform),
                                                         glm::value_ptr(translation),
                                                         glm::value_ptr(rotation),
                                                         glm::value_ptr(scale));

                    transformComponent.SetLocalPosition(glm::vec3(translation.x, translation.y, 0.f));
                    transformComponent.SetLocalRotation(glm::vec3(0.f, 0.f, rotation.z));
                    transformComponent.SetLocalScale(glm::vec3(scale.x, scale.y, 1.f));
                }
            }
            else
            {
                ImGuizmo::SetGizmoSizeClipSpace(0.2);

                // Customize ImGuizmo style to be more similar to Godot

                auto& style = ImGuizmo::GetStyle();

                // Set colors
                style.Colors[ImGuizmo::DIRECTION_X] = ImVec4(0.918f, 0.196f, 0.310f, 1.0f);
                style.Colors[ImGuizmo::DIRECTION_Y] = ImVec4(0.153f, 0.525f, 0.918f, 1.0f);
                style.Colors[ImGuizmo::DIRECTION_Z] = ImVec4(0.502f, 0.800f, 0.051f, 1.0f);
                style.Colors[ImGuizmo::PLANE_X] = ImVec4(0.918f, 0.196f, 0.310f, 1.0f);
                style.Colors[ImGuizmo::PLANE_Y] = ImVec4(0.153f, 0.525f, 0.918f, 1.0f);
                style.Colors[ImGuizmo::PLANE_Z] = ImVec4(0.502f, 0.800f, 0.051f, 1.0f);
                style.Colors[ImGuizmo::SELECTION] = ImVec4(1.0f, 1.0f, 0.0f, 1.0f);

                ImGuizmo::SetOrthographic(false);
                ImGuizmo::SetDrawlist();

                ImGuizmo::SetRect(m_ViewportBounds[0].x, m_ViewportBounds[0].y, m_ViewportBounds[1].x - m_ViewportBounds[0].x, m_ViewportBounds[1].y - m_ViewportBounds[0].y);

                const glm::mat4& cameraProjection = m_EditorCamera.GetProjection();
                glm::mat4 cameraView = m_EditorCamera.GetViewMatrix();

                auto& transformComponent = selectedEntity.GetComponent<TransformComponent>();
                glm::mat4 transform = transformComponent.GetWorldTransform();

                ImGuizmo::Manipulate(glm::value_ptr(cameraView), glm::value_ptr(cameraProjection),
                                    (ImGuizmo::OPERATION)m_GizmoType, ImGuizmo::LOCAL,
                             glm::value_ptr(transform));

                if (ImGuizmo::IsUsing())
                {
                  /*TODO: Revisit this bc this should work using the SetWorldTransform
                    but for this in the SetWorldTransform we should update the local
                    transform too and for this we need the transform of the parent.*/

                    glm::mat4 localTransform = transform;

                    auto& parentEntity = selectedEntity.GetComponent<HierarchyComponent>().m_Parent;
                    if(parentEntity != entt::null)
                    {
                        Entity e{parentEntity, m_Context.sceneManager->GetActiveScene().get()};
                        glm::mat4 parentGlobalTransform = e.GetComponent<TransformComponent>().GetWorldTransform();
                        glm::mat4 inverseParentGlobalTransform = glm::inverse(parentGlobalTransform);
                        localTransform = inverseParentGlobalTransform * transform;
                    }

                    // Update the local transform component
                    transformComponent.SetLocalTransform(localTransform);
                }
            }
        }
        else
        {
            m_GizmoType = ImGuizmo::OPERATION::TRANSLATE;
        }

        //transparent overlay displaying fps draw calls etc
        ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoDocking | /*ImGuiWindowFlags_AlwaysAutoResize |*/ ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoFocusOnAppearing | ImGuiWindowFlags_NoNav;

        ImGui::SetNextWindowPos(ImVec2(ImGui::GetWindowPos().x + ImGui::GetWindowSize().x - 205, ImGui::GetWindowPos().y + ImGui::GetWindowSize().y - 100));

        ImGui::SetNextWindowBgAlpha(0.35f); // Transparent background

        ImGui::Begin("Renderer Stats", NULL, window_flags);
        ImGui::Text("Size: %.0f x %.0f (%0.1fMP)", m_ViewportSize.x, m_ViewportSize.y, m_ViewportSize.x * m_ViewportSize.y / 1000000.0f);
        ImGui::Text("Draw Calls: %d", m_Context.renderer->Get3DRenderer().GetStats().DrawCalls);
        ImGui::Text("Vertex Count: %d", m_Context.renderer->Get3DRenderer().GetStats().VertexCount);
        ImGui::Text("Index Count: %d", m_Context.renderer->Get3DRenderer().GetStats().IndexCount);
        ImGui::End();

        // Display EditorCamera speed vertical slider & zoom vertical slider at the center left

        auto DrawVerticalProgressBar = [&](float value, const ImVec4& color, float min = 0.0f, float max = 1.0f) {
            window_flags = ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoDocking | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoFocusOnAppearing | ImGuiWindowFlags_NoNav | ImGuiWindowFlags_NoBackground;

            float windowHeight = ImGui::GetWindowHeight();
            float sliderHeight = windowHeight * 0.5f;

            ImGui::SetNextWindowPos(ImVec2(ImGui::GetWindowPos().x + 10, ImGui::GetWindowPos().y + (ImGui::GetWindowSize().y / 2) - (sliderHeight / 2)));

            ImGui::SetNextWindowBgAlpha(0.0f); // Transparent background

            ImGui::Begin("##Speed Slider", NULL, window_flags);

            ImGui::PushStyleColor(ImGuiCol_FrameBg, color);

            ImGui::PushItemFlag(ImGuiItemFlags_Disabled, true);
            ImGui::VSliderFloat("##speed", ImVec2(10, sliderHeight), &value, min, max, "");
            ImGui::PopItemFlag();

            ImGui::PopStyleColor();
            ImGui::End();
        };

        ImVec4 color = ImGui::GetStyleColorVec4(ImGuiCol_FrameBg);
        color.w = 0.5f;
        switch(m_EditorCamera.GetState())
        {
            using enum EditorCamera::CameraState;
            case FLY:
                DrawVerticalProgressBar(m_EditorCamera.GetFlySpeed(), color);
                break;
            case ORBIT:
                DrawVerticalProgressBar(100 - m_EditorCamera.GetOrbitZoom(), color, 1.0f, 100.0f);
                break;
            case NONE:
                // TODO when we refractor the EditorCamera class, we should use this case to display the slider when the camera is not moving
                break;
        }

        // End of EditorCamera ----------------------------

        // Render Mode Button Viewport Overlay
        ImGui::SetNextWindowPos(ImVec2(ImGui::GetWindowPos().x + 15, ImGui::GetWindowPos().y + 35));
        ImGui::SetNextWindowBgAlpha(0.35f); // Transparent background
        ImGui::Begin("Render Mode", NULL, window_flags);
        static bool a = false;
        if(ImGui::Button("Render Mode", ImVec2(100, 32)))
            ImGui::OpenPopup("Render Mode Popup");
        if(ImGui::BeginPopup("Render Mode Popup", ImGuiWindowFlags_AlwaysAutoResize))
        {
            ImGui::BeginDisabled(true);
            ImGui::Checkbox("Wireframe", &a);
            ImGui::Checkbox("Solid", &a);
            ImGui::Checkbox("Lighting", &a);
            ImGui::EndDisabled();
            static bool normals = false;
            ImGui::Checkbox("Normals", &normals);
            m_Context.renderer->Get3DRenderer().GetRenderSettings().showNormals = normals;
            m_Context.renderer->GetRenderSettings().PostProcessing = !normals;
            ImGui::EndPopup();
        }
        ImGui::End();

        ImGui::End();
        ImGui::PopStyleVar();
    }

    void EditorLayer::OnOverlayRender()
    {
        m_Context.renderer->SetCurrentRenderTarget(m_ViewportRenderTarget.get());

        Entity selectedEntity = m_SceneTreePanel.GetSelectedEntity();
        static Entity lastSelectedEntity;  

        if(selectedEntity)
        {
            auto& transformComponent = selectedEntity.GetComponent<TransformComponent>();
            if (selectedEntity.HasComponent<MeshComponent>()) {
                auto& meshComponent = selectedEntity.GetComponent<MeshComponent>();

                glm::mat4 transform = transformComponent.GetWorldTransform();

                if(meshComponent.drawAABB)
                {
                    const AABB& aabb = meshComponent.mesh ? meshComponent.mesh->GetAABB().CalculateTransformedAABB(transform) : AABB();
                    m_Context.renderer->Get2DRenderer().DrawBox(aabb, {0.27f, 0.52f, 0.53f, 1.0f});
                }

                // ----------------------------------

                OBB obb = meshComponent.mesh ? meshComponent.mesh->GetOBB(transform) : OBB();
                m_Context.renderer->Get2DRenderer().DrawBox(obb, {0.99f, 0.50f, 0.09f, 1.0f});


            }
            else if (selectedEntity != lastSelectedEntity)
            {
                // TODO generate defaults bounding boxes for when the entity does not have a mesh component
                lastSelectedEntity = selectedEntity;
                COFFEE_CORE_WARN("Not printing bounding box: Selected entity {0} does not have a MeshComponent.", selectedEntity.GetComponent<TagComponent>().Tag);
            }

        }

        auto view = m_Context.sceneManager->GetActiveScene()->GetAllEntitiesWithComponents<LightComponent, TransformComponent>();

        for(auto entity : view)
        {
            auto& lightComponent = view.get<LightComponent>(entity);
            auto& transformComponent = view.get<TransformComponent>(entity);

            switch (lightComponent.type) {
                case LightComponent::Type::DirectionalLight:
                    //m_Context.renderer->Get2DRenderer().DrawArrow(transformComponent.GetWorldTransform()[3], lightComponent.Direction, lightComponent.Intensity);
                    m_Context.renderer->Get2DRenderer().DrawArrow(transformComponent.GetWorldTransform()[3], lightComponent.Direction, 1.5f);
                break;

                case LightComponent::Type::PointLight:
                    glm::vec3 worldPosition = transformComponent.GetWorldTransform()[3];
                    float radius = lightComponent.Range;
                    m_Context.renderer->Get2DRenderer().DrawSphere(worldPosition, radius);
                break;

                /* case LightComponent::Type::SpotLight:
                break;    */         
            }
        }

        auto cameraView = m_Context.sceneManager->GetActiveScene()->GetAllEntitiesWithComponents<CameraComponent, TransformComponent>();

        for(auto entity : cameraView)
        {
            auto& cameraComponent = cameraView.get<CameraComponent>(entity);
            auto& transformComponent = cameraView.get<TransformComponent>(entity);

            glm::mat4 viewProjection = cameraComponent.Camera.GetProjection() * glm::inverse(transformComponent.GetWorldTransform());

            m_Context.renderer->Get2DRenderer().DrawFrustum(viewProjection, {0.99f, 0.50f, 0.09f, 1.0f});
        }

        m_Context.renderer->Get2DRenderer().DrawLine({-1000.0f, 0.0f, 0.0f}, {1000.0f, 0.0f, 0.0f}, {0.918f, 0.196f, 0.310f, 1.0f}, 2);
        m_Context.renderer->Get2DRenderer().DrawLine({0.0f, -1000.0f, 0.0f}, {0.0f, 1000.0f, 0.0f}, {0.502f, 0.800f, 0.051f, 1.0f}, 2);
        m_Context.renderer->Get2DRenderer().DrawLine({0.0f, 0.0f, -1000.0f}, {0.0f, 0.0f, 1000.0f}, {0.153f, 0.525f, 0.918f, 1.0f}, 2);

        static Ref<Mesh> gridPlane = PrimitiveMesh::CreatePlane({1000.0f, 1000.0f});
        static Ref<Shader> gridShader = Shader::Create("assets/shaders/SimpleGridShader.glsl");
        static Ref<Material> gridShaderMaterial = ShaderMaterial::Create("GridShaderMaterialFront", gridShader);

        MaterialRenderSettings& gridMaterialRenderSettings = gridShaderMaterial->GetRenderSettings();
        gridMaterialRenderSettings.cullMode = MaterialRenderSettings::CullMode::None;
        gridMaterialRenderSettings.transparencyMode = MaterialRenderSettings::TransparencyMode::Alpha;

        m_Context.renderer->Get3DRenderer().Submit(RenderCommand{.mesh = gridPlane, .material = gridShaderMaterial});
    }

    void EditorLayer::ResizeViewport(float width, float height)
    {
        if((m_ViewportSize.x > 0.0f && m_ViewportSize.y > 0.0f) &&
           (width != m_ViewportSize.x || height != m_ViewportSize.y))
        {
            m_EditorCamera.SetViewportSize(width, height);
            
            // Apply render scale to actual framebuffer resolution
            float renderScale = m_Context.renderer->GetRenderSettings().RenderScale;
            uint32_t scaledWidth = static_cast<uint32_t>(width * renderScale);
            uint32_t scaledHeight = static_cast<uint32_t>(height * renderScale);
            m_ViewportRenderTarget->Resize(scaledWidth, scaledHeight);
        }

        m_ViewportSize = { width, height };
    }

    void EditorLayer::OnScenePlay()
    {
        if(m_Context.sceneManager->GetActiveScene()->GetFilePath().empty())
        {
            COFFEE_ERROR("Scene is not saved! Please save the scene before playing.");
            return;
        }

        m_Context.sceneManager->SetSceneState(SceneManager::SceneState::Play);

        Scene::Save(m_Context.sceneManager->GetActiveScene()->GetFilePath(), m_Context.sceneManager->GetActiveScene());

        m_Context.sceneManager->ChangeScene(m_Context.sceneManager->GetActiveScene()->GetFilePath());

        m_SceneTreePanel.SetContext(m_Context.sceneManager->GetActiveScene());
        m_SceneTreePanel.SetSelectedEntity(Entity());
        m_ContentBrowserPanel.SetContext(m_Context.sceneManager->GetActiveScene());
    }

    void EditorLayer::OnSceneStop()
    {
        COFFEE_CORE_ASSERT(m_Context.sceneManager->GetSceneState() == m_Context.sceneManager->SceneState::Play)
        

        m_Context.sceneManager->SetSceneState(SceneManager::SceneState::Edit);

        m_Context.sceneManager->ChangeScene(m_EditorScene);

        m_SceneTreePanel.SetContext(m_Context.sceneManager->GetActiveScene());
        m_SceneTreePanel.SetSelectedEntity(Entity());
        m_ContentBrowserPanel.SetContext(m_Context.sceneManager->GetActiveScene());
    }

    void EditorLayer::NewProject()
    {
        FileDialogArgs args;
        args.Filters = {{"Coffee Project", "TeaProject"}};
        args.DefaultName = "Untitled.TeaProject";
        const std::filesystem::path& path = FileDialog::SaveFile(args);

        if (!path.empty())
        {
            m_Context.projectManager->NewProject(path);
            m_Context.projectManager->SaveCurrentProject();
            m_Context.projectManager->LoadProject(path);
            m_Context.window->SetTitle(m_Context.projectManager->GetCurrentProject()->GetName() + " - Coffee Engine");
        }
        else
        {
            COFFEE_CORE_ERROR("New Project: No file selected!");
        }
    }

    void EditorLayer::OpenProject()
    {
        FileDialogArgs args;
        args.Filters = {{"Coffee Project", "TeaProject"}};
        const std::filesystem::path& path = FileDialog::OpenFile(args);

        if (!path.empty())
        {
            m_Context.projectManager->LoadProject(path);
            m_Context.window->SetTitle(m_Context.projectManager->GetCurrentProject()->GetName() + " - Coffee Engine");
        }
        else
        {
            COFFEE_CORE_WARN("Open Project: No file selected");
        }
    }

    void EditorLayer::SaveProject()
    {
        m_Context.projectManager->SaveCurrentProject();
    }

    void EditorLayer::NewScene()
    {
        m_EditorScene = CreateRef<Scene>(const_cast<EngineContext&>(m_Context));
        m_Context.sceneManager->ChangeScene(m_EditorScene);

        m_SceneTreePanel = SceneTreePanel();

        m_SceneTreePanel.SetContext(m_Context.sceneManager->GetActiveScene());
        m_ContentBrowserPanel.SetContext(m_Context.sceneManager->GetActiveScene());
        m_ContentBrowserPanel.SetEngineContext(m_Context);
        m_ImportPanel.SetEngineContext(m_Context);
        m_ProjectSettingsPanel.SetContext(m_Context);
    }

    void EditorLayer::OpenScene()
    {
        FileDialogArgs args;
        args.Filters = {{"Coffee Scene", "TeaScene"}};
        const std::filesystem::path& path = FileDialog::OpenFile(args);

        if (!path.empty() and path.extension() == ".TeaScene")
        {
            m_EditorScene = Scene::Load(path, const_cast<EngineContext&>(m_Context));
            m_Context.sceneManager->ChangeScene(m_EditorScene);

            m_SceneTreePanel = SceneTreePanel();

            m_SceneTreePanel.SetContext(m_Context.sceneManager->GetActiveScene());
            m_ContentBrowserPanel.SetContext(m_Context.sceneManager->GetActiveScene());
            m_ContentBrowserPanel.SetEngineContext(m_Context);
            m_ImportPanel.SetEngineContext(m_Context);
            m_ProjectSettingsPanel.SetContext(m_Context);
        }
        else
        {
            COFFEE_CORE_WARN("Open Scene: No file selected");
        }
    }
    void EditorLayer::SaveScene()
    {
        FileDialogArgs args;
        args.Filters = {{"Coffee Scene", "TeaScene"}};
        const std::filesystem::path& path = FileDialog::SaveFile(args);

        if (!path.empty())
        {
            Scene::Save(path, m_Context.sceneManager->GetActiveScene());
        }
        else
        {
            COFFEE_CORE_WARN("Save Scene: No file selected");
        }

        /* Scene::Save(m_Context.projectManager->GetCurrentProject()->GetProjectDirectory() / "Untitled.TeaScene", m_Context.sceneManager->GetActiveScene()); */
    }
    void EditorLayer::SaveSceneAs() {}

}
