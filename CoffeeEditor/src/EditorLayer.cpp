#include "EditorLayer.h"

#include "CoffeeEngine/Core/Application.h"
#include "CoffeeEngine/Core/Assert.h"
#include "CoffeeEngine/Core/Base.h"
#include "CoffeeEngine/Core/FileDialog.h"
#include "CoffeeEngine/Core/Input.h"
#include "CoffeeEngine/Core/Log.h"
#include "CoffeeEngine/Core/MouseCodes.h"
#include "CoffeeEngine/Events/ApplicationEvent.h"
#include "CoffeeEngine/Events/KeyEvent.h"
#include "CoffeeEngine/IO/ResourceLoader.h"
#include "CoffeeEngine/IO/ResourceRegistry.h"
#include "CoffeeEngine/IO/ResourceUtils.h"
#include "CoffeeEngine/Project/Project.h"
#include "CoffeeEngine/Renderer/EditorCamera.h"
#include "CoffeeEngine/Renderer/Framebuffer.h"
#include "CoffeeEngine/Renderer/RenderTarget.h"
#include "CoffeeEngine/Renderer/Renderer.h"
#include "CoffeeEngine/Renderer/Renderer2D.h"
#include "CoffeeEngine/Renderer/Renderer3D.h"
#include "CoffeeEngine/Scene/Components.h"
#include "CoffeeEngine/Scene/PrimitiveMesh.h"
#include "CoffeeEngine/Scene/Scene.h"
#include "CoffeeEngine/Scene/SceneCamera.h"
#include "CoffeeEngine/Scene/SceneTree.h"
#include "CoffeeEngine/Scripting/Lua/LuaBackend.h"
#include "CoffeeEngine/Scripting/ScriptManager.h"
#include "Panels/SceneTreePanel.h"
#include "entt/entity/entity.hpp"
#include "imgui_internal.h"
#include "CoffeeEngine/Scene/SceneManager.h"

#include <ImGuizmo.h>
#include <cstdint>
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

    EditorLayer::EditorLayer() : Layer("Example")
    {

    }

    void EditorLayer::OnAttach()
    {
        ZoneScoped;

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
                                                            {1280, 720}, 
                                        EditorViewportRenderTargetFramebufferAttachments);

        ScriptManager::RegisterBackend(ScriptingLanguage::Lua, CreateRef<LuaBackend>());

        m_EditorScene = CreateRef<Scene>();
            
        SceneManager::SetSceneState(SceneManager::SceneState::Edit);
        SceneManager::ChangeScene(m_EditorScene);

        m_EditorCamera = EditorCamera(45.0f);

        m_SceneTreePanel.SetContext(SceneManager::GetActiveScene());
        m_ContentBrowserPanel.SetContext(SceneManager::GetActiveScene());
    }

    void EditorLayer::OnUpdate(float dt)
    {
        ZoneScoped;
        
        // Idk if this is the best place or is better in each switch case for flexibility
        Renderer::SetCurrentRenderTarget(m_ViewportRenderTarget);

        switch (SceneManager::GetSceneState())
        {
            using enum SceneManager::SceneState;

            case Edit:
                m_EditorCamera.OnUpdate(dt);
                SceneManager::GetActiveScene()->OnUpdateEditor(m_EditorCamera, dt);
                OnOverlayRender();
            break;
            case Play:
                SceneManager::GetActiveScene()->OnUpdateRuntime(dt);
            break;

        }
        
        Renderer::SetCurrentRenderTarget(nullptr);
    }

    void EditorLayer::OnEvent(Coffee::Event& event)
    {
        ZoneScoped;

        m_EditorCamera.OnEvent(event);

        SceneManager::GetActiveScene()->OnEvent(event);

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
                glm::vec2 mousePos = Input::GetMousePosition();
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

                    Entity hoveredEntity = entityID == 16777215 ? Entity() : Entity((entt::entity)entityID, SceneManager::GetActiveScene().get());

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
        const std::filesystem::path& projectDirectory = Project::GetActive()->GetProjectDirectory();
        const std::filesystem::path& originPath = event.GetFile();
        const std::filesystem::path& destFilePath = projectDirectory / originPath.filename();
        std::filesystem::copy(originPath, destFilePath, std::filesystem::copy_options::recursive);
        
        if(std::filesystem::is_directory(destFilePath))
        {
            ResourceLoader::LoadDirectory(destFilePath);
        }
        else
        {
            ResourceLoader::LoadFile(destFilePath);
        }
        return false;
    }

    void EditorLayer::OnDetach()
    {
        ZoneScoped;

        SceneManager::GetActiveScene()->OnExitEditor();
    }

    void EditorLayer::OnImGuiRender()
    {
        ZoneScoped;

        ImGui::DockSpaceOverViewport(0, ImGui::GetMainViewport());

        static bool show = true;
        ImGui::ShowDemoWindow(&show);

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
                if (ImGui::MenuItem(ICON_LC_SAVE " Save Scene As...", "Ctrl+Shift+S")) { SaveSceneAs(); }
                if (ImGui::MenuItem(ICON_LC_X " Exit")) { Application::Get().Close(); }
                ImGui::EndMenu();
            }
            if (ImGui::BeginMenu("Project"))
            {
                if (ImGui::MenuItem(ICON_LC_FILE_PLUS_2 " New Project...", "Ctrl+N")) { NewProject(); }
                if (ImGui::MenuItem(ICON_LC_FOLDER_OPEN " Open Project...", "Ctrl+O")) { OpenProject(); }
                if (ImGui::MenuItem(ICON_LC_SAVE " Save Project", "Ctrl+S")) { SaveProject(); }
                if(ImGui::MenuItem(ICON_LC_SETTINGS " Project Settings", nullptr, mainMenuWindows.ProjectSettings)) 
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
                        Application::Get().GetImGuiLayer()->SetCoffeeColorStyle();
                    }
                    if(ImGui::MenuItem("Godot"))
                    {
                        Application::Get().GetImGuiLayer()->SetGodotColorStyle();
                    }
                    if(ImGui::MenuItem("Tea"))
                    {
                        Application::Get().GetImGuiLayer()->SetTeaColorStyle();
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

            switch (SceneManager::GetSceneState())
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
            ImVec2 textSize = ImGui::CalcTextSize(("FPS:" + std::to_string(Application::Get().GetFPS())).c_str());
            ImGui::SetCursorPosX(ImGui::GetWindowWidth() - textSize.x);
            ImGui::TextDisabled("FPS: %.1f", Application::Get().GetFPS());

            ImGui::EndMainMenuBar();
        }

        if(mainMenuWindows.ProjectSettings)
        {
            ImGui::Begin("Project Settings");
            ImGui::Text("Project Settings");
            ImGui::Separator();
            ImGui::End();
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

        Application::Get().GetImGuiLayer()->BlockEvents(!m_ViewportHovered);

        ImVec2 viewportPanelSize = ImGui::GetContentRegionAvail();
        ResizeViewport(viewportPanelSize.x, viewportPanelSize.y);

        uint32_t textureID = m_ViewportRenderTarget->GetFramebuffer("Forward")->GetColorTexture("Color")->GetID();
        ImGui::Image((void*)textureID, ImVec2{ m_ViewportSize.x, m_ViewportSize.y }, {0, 1}, {1, 0});

        //Guizmo
        Entity selectedEntity = m_SceneTreePanel.GetSelectedEntity();

        if(selectedEntity and m_GizmoType != -1 and SceneManager::GetSceneState() == SceneManager::SceneState::Edit)
        {
            ImGuizmo::SetGizmoSizeClipSpace(0.2);

            // Customize ImGuizmo style to be more similar to Godot

            auto& style = ImGuizmo::GetStyle();

            //style.TranslationLineThickness = 3.0f;
            //style.TranslationLineArrowSize = 10.0f;
            //style.RotationLineThickness = 4.0f;
            //style.RotationOuterLineThickness = 4.0f;
            //style.ScaleLineThickness = 4.0f;
            //style.ScaleLineCircleSize = 6.0f;

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
                    Entity e{parentEntity, SceneManager::GetActiveScene().get()};
                    glm::mat4 parentGlobalTransform = e.GetComponent<TransformComponent>().GetWorldTransform();
                    glm::mat4 inverseParentGlobalTransform = glm::inverse(parentGlobalTransform);
                    localTransform = inverseParentGlobalTransform * transform;
                }

                // Update the local transform component
                transformComponent.SetLocalTransform(localTransform);
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
        ImGui::Text("Draw Calls: %d", Renderer3D::GetStats().DrawCalls);
        ImGui::Text("Vertex Count: %d", Renderer3D::GetStats().VertexCount);
        ImGui::Text("Index Count: %d", Renderer3D::GetStats().IndexCount);
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
            Renderer3D::GetRenderSettings().showNormals = normals;
            Renderer::GetRenderSettings().PostProcessing = !normals;
            ImGui::EndPopup();
        }
        ImGui::End();

        ImGui::End();
        ImGui::PopStyleVar();

        //---------TESTING---------
        ImGui::Begin("Render Settings");

        ImGui::Checkbox("Post Processing", &Renderer::GetRenderSettings().PostProcessing);

        ImGui::DragFloat("Exposure", &Renderer3D::GetRenderSettings().Exposure, 0.001f, 100.0f);

        ImGui::End();

        // Debug Window for testing the ResourceRegistry
        ImGui::Begin("Resource Registry");
        
        // Static variable to store the search query
        static std::string searchQuery;
        
        // Input text field for the search query
        char buffer[256];
        strncpy(buffer, searchQuery.c_str(), sizeof(buffer));
        if (ImGui::InputText("Search", buffer, sizeof(buffer)))
        {
            searchQuery = std::string(buffer);
        }
        
        if (ImGui::BeginTable("ResourceTable", 4, ImGuiTableFlags_Resizable | ImGuiTableFlags_ScrollY | ImGuiTableFlags_Borders | ImGuiTableFlags_RowBg | ImGuiTableFlags_Sortable))
        {
            ImGui::TableSetupColumn("Name", ImGuiTableColumnFlags_DefaultSort);
            ImGui::TableSetupColumn("UUID", ImGuiTableColumnFlags_DefaultSort);
            ImGui::TableSetupColumn("Type", ImGuiTableColumnFlags_DefaultSort);
            ImGui::TableSetupColumn("Use Count", ImGuiTableColumnFlags_DefaultSort);
            ImGui::TableHeadersRow();
        
            auto& resources = ResourceRegistry::GetResourceRegistry();
            for (auto& resource : resources)
            {
                // Filter resources based on the search query
                if (searchQuery.empty() || resource.second->GetName().find(searchQuery) != std::string::npos)
                {
                    ImGui::TableNextRow();
                    ImGui::TableSetColumnIndex(0);
                    ImGui::Text("%s", resource.second->GetName().c_str());
                    ImGui::TableSetColumnIndex(1);
                    ImGui::Text("%lu", resource.first);
                    ImGui::TableSetColumnIndex(2);
                    ImGui::Text("%s", ResourceTypeToString(resource.second->GetType()).c_str());
                    ImGui::TableSetColumnIndex(3);
                    ImGui::Text("%d", resource.second.use_count());
                }
            }
        
            ImGui::EndTable();
        }
        
        ImGui::End();
    }

    void EditorLayer::OnOverlayRender()
    {
        Renderer::SetCurrentRenderTarget(m_ViewportRenderTarget);

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
                    Renderer2D::DrawBox(aabb, {0.27f, 0.52f, 0.53f, 1.0f});
                }

                // ----------------------------------

                OBB obb = meshComponent.mesh ? meshComponent.mesh->GetOBB(transform) : OBB();
                Renderer2D::DrawBox(obb, {0.99f, 0.50f, 0.09f, 1.0f});


            }
            else if (selectedEntity != lastSelectedEntity)
            {
                // TODO generate defaults bounding boxes for when the entity does not have a mesh component
                lastSelectedEntity = selectedEntity;
                COFFEE_CORE_WARN("Not printing bounding box: Selected entity {0} does not have a MeshComponent.", selectedEntity.GetComponent<TagComponent>().Tag);
            }

        }

        auto view = SceneManager::GetActiveScene()->GetAllEntitiesWithComponents<LightComponent, TransformComponent>();

        for(auto entity : view)
        {
            auto& lightComponent = view.get<LightComponent>(entity);
            auto& transformComponent = view.get<TransformComponent>(entity);

            switch (lightComponent.type) {
                case LightComponent::Type::DirectionalLight:
                    //Renderer2D::DrawArrow(transformComponent.GetWorldTransform()[3], lightComponent.Direction, lightComponent.Intensity);
                    Renderer2D::DrawArrow(transformComponent.GetWorldTransform()[3], lightComponent.Direction, 1.5f);
                break;

                case LightComponent::Type::PointLight:
                    glm::vec3 worldPosition = transformComponent.GetWorldTransform()[3];
                    float radius = lightComponent.Range;
                    Renderer2D::DrawSphere(worldPosition, radius);
                break;

                /* case LightComponent::Type::SpotLight:
                break;    */         
            }
        }

        auto cameraView = SceneManager::GetActiveScene()->GetAllEntitiesWithComponents<CameraComponent, TransformComponent>();

        for(auto entity : cameraView)
        {
            auto& cameraComponent = cameraView.get<CameraComponent>(entity);
            auto& transformComponent = cameraView.get<TransformComponent>(entity);

            glm::mat4 viewProjection = cameraComponent.Camera.GetProjection() * glm::inverse(transformComponent.GetWorldTransform());

            Renderer2D::DrawFrustum(viewProjection, {0.99f, 0.50f, 0.09f, 1.0f});
        }

        Renderer2D::DrawLine({-1000.0f, 0.0f, 0.0f}, {1000.0f, 0.0f, 0.0f}, {0.918f, 0.196f, 0.310f, 1.0f}, 2);
        Renderer2D::DrawLine({0.0f, -1000.0f, 0.0f}, {0.0f, 1000.0f, 0.0f}, {0.502f, 0.800f, 0.051f, 1.0f}, 2);
        Renderer2D::DrawLine({0.0f, 0.0f, -1000.0f}, {0.0f, 0.0f, 1000.0f}, {0.153f, 0.525f, 0.918f, 1.0f}, 2);

        static Ref<Mesh> gridPlaneDown = PrimitiveMesh::CreatePlane({1000.0f, 1000.0f});
        static Ref<Mesh> gridPlaneUp = PrimitiveMesh::CreatePlane({1000.0f, -1000.0f}); // FIXME this is a hack to avoid the grid not beeing rendered due to backface culling
        static Ref<Shader> gridShader = Shader::Create("assets/shaders/SimpleGridShader.glsl");

        Renderer3D::Submit(gridShader, gridPlaneUp->GetVertexArray());
        Renderer3D::Submit(gridShader, gridPlaneDown->GetVertexArray());
    }

    void EditorLayer::ResizeViewport(float width, float height)
    {
        if((m_ViewportSize.x > 0.0f && m_ViewportSize.y > 0.0f) &&
           (width != m_ViewportSize.x || height != m_ViewportSize.y))
        {
            m_EditorCamera.SetViewportSize(width, height);
            m_ViewportRenderTarget->Resize((uint32_t)width, (uint32_t)height);
        }

        m_ViewportSize = { width, height };
    }

    void EditorLayer::OnScenePlay()
    {
        if(SceneManager::GetActiveScene()->GetFilePath().empty())
        {
            COFFEE_ERROR("Scene is not saved! Please save the scene before playing.");
            return;
        }

        SceneManager::SetSceneState(SceneManager::SceneState::Play);

        Scene::Save(SceneManager::GetActiveScene()->GetFilePath(), SceneManager::GetActiveScene());

        SceneManager::ChangeScene(SceneManager::GetActiveScene()->GetFilePath());

        m_SceneTreePanel.SetContext(SceneManager::GetActiveScene());
        m_SceneTreePanel.SetSelectedEntity(Entity());
        m_ContentBrowserPanel.SetContext(SceneManager::GetActiveScene());
    }

    void EditorLayer::OnSceneStop()
    {
        COFFEE_CORE_ASSERT(SceneManager::GetSceneState() == SceneManager::SceneState::Play)
        

        SceneManager::SetSceneState(SceneManager::SceneState::Edit);

        SceneManager::ChangeScene(m_EditorScene);

        m_SceneTreePanel.SetContext(SceneManager::GetActiveScene());
        m_SceneTreePanel.SetSelectedEntity(Entity());
        m_ContentBrowserPanel.SetContext(SceneManager::GetActiveScene());
    }

    void EditorLayer::NewProject()
    {
        FileDialogArgs args;
        args.Filters = {{"Coffee Project", "TeaProject"}};
        args.DefaultName = "Untitled.TeaProject";
        const std::filesystem::path& path = FileDialog::SaveFile(args);

        if (!path.empty())
        {
            Project::New(path);
            Project::SaveActive();
            Project::Load(path);
            Application::Get().GetWindow().SetTitle(Project::GetActive()->GetProjectName() + " - Coffee Engine");
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
            Project::Load(path);
            Application::Get().GetWindow().SetTitle(Project::GetActive()->GetProjectName() + " - Coffee Engine");
        }
        else
        {
            COFFEE_CORE_WARN("Open Project: No file selected");
        }
    }

    void EditorLayer::SaveProject()
    {
        Project::SaveActive();
    }

    void EditorLayer::NewScene()
    {
        AudioZone::RemoveAllReverbZones();
        Audio::UnregisterAllGameObjects();

        m_EditorScene = CreateRef<Scene>();
        SceneManager::ChangeScene(m_EditorScene);

        m_SceneTreePanel = SceneTreePanel();

        m_SceneTreePanel.SetContext(SceneManager::GetActiveScene());
        m_ContentBrowserPanel.SetContext(SceneManager::GetActiveScene());
    }

    void EditorLayer::OpenScene()
    {
        FileDialogArgs args;
        args.Filters = {{"Coffee Scene", "TeaScene"}};
        const std::filesystem::path& path = FileDialog::OpenFile(args);

        if (!path.empty() and path.extension() == ".TeaScene")
        {
            AudioZone::RemoveAllReverbZones();
            Audio::UnregisterAllGameObjects();

            m_EditorScene = Scene::Load(path);
            SceneManager::ChangeScene(m_EditorScene);

            m_SceneTreePanel = SceneTreePanel();

            m_SceneTreePanel.SetContext(SceneManager::GetActiveScene());
            m_ContentBrowserPanel.SetContext(SceneManager::GetActiveScene());
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
            Scene::Save(path, SceneManager::GetActiveScene());
        }
        else
        {
            COFFEE_CORE_WARN("Save Scene: No file selected");
        }

        /* Scene::Save(Project::GetActive()->GetProjectDirectory() / "Untitled.TeaScene", SceneManager::GetActiveScene()); */
    }
    void EditorLayer::SaveSceneAs() {}

}
