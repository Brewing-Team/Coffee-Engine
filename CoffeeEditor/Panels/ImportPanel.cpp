#include "ImportPanel.h"

#include "CoffeeEngine/Core/Application.h"
#include "CoffeeEngine/IO/ImportSettings.h"
#include "imgui.h"

#include <fstream>
#include <src/EditorLayer.h>

namespace Coffee {

    ImportPanel::ImportPanel(const Ref<Scene>& scene)
    {
        m_Context = scene;
    }

    void ImportPanel::SetContext(const Ref<Scene>& scene)
    {
        m_Context = scene;
    }

    void ImportPanel::OnImGuiRender()
    {
        if (!m_Visible) return;

        if (ImGui::Button("Reimport")) {
            HandleReimport();
        }

        ImGui::Begin("Import");

        if (m_Context)
        {
            if (selectedResource)
            {
                ResourceType type = selectedResource->GetType();

                ImGui::Text("Selected Resource: %s", selectedResource->GetName().c_str());
                ImGui::Separator();
                switch (type)
                {
                    using enum ResourceType;
                    case Unknown:
                        break;
                    case Texture:
                        ImGui::Text("Texture Properties");
                        ImGui::Checkbox("sRGB", &srgb);
                        ImGui::Checkbox("Flip Y", &flipY);
                        ImGui::Checkbox("Flip X", &flipX);

                        ImGui::Separator();
                        ImGui::Button("Reimport");

                        break;
                    case Texture2D:
                        ImGui::Text("Texture Properties");
                        ImGui::Checkbox("sRGB", &srgb);
                        ImGui::Checkbox("Flip Y", &flipY);
                        ImGui::Checkbox("Flip X", &flipX);

                        ImGui::Separator();
                        ImGui::Button("Reimport");

                        break;
                    case Cubemap:
                        break;
                case Model:
                        ImGui::Text("Model Properties");
                        ImGui::Separator();
                        ImGui::InputFloat("Global Scale", &globalScale);

                        ImGui::Text("Camera Properties");
                        ImGui::Separator();
                        ImGui::Checkbox("Ignore Cameras", &ignoreCamera);

                        ImGui::Text("Light Properties");
                        ImGui::Separator();
                        ImGui::Checkbox("Ignore Lights", &ignoreLight);

                        ImGui::Separator();
                        ImGui::Button("Reimport");

                        break;
                    case Mesh:

                        ImGui::Separator();
                        ImGui::Button("Reimport");
                        break;
                    case Shader:

                        ImGui::Separator();
                        ImGui::Button("Reimport");
                        break;
                    case Material:

                        ImGui::Separator();
                        ImGui::Button("Reimport");
                        break;
                }
            }
            else
            {
                ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.0f, 0.0f, 0.0f, 1.0f));
                ImGui::TextWrapped("No resource selected!");
                ImGui::PopStyleColor();

                ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.5f, 0.5f, 0.5f, 1.0f));
                ImGui::TextWrapped("Select a resource from the content browser to adjust its properties");
                ImGui::PopStyleColor();
            }
        }

        ImGui::End();
    }

    void ImportPanel::HandleReimport() {
        if (!selectedResource || !m_CurrentSettings)
            return;

        ResourceImporter importer;
        bool success = importer.ReimportResource(selectedResource, *m_CurrentSettings);

        if (success)
            COFFEE_INFO("Resource reimported successfully");
        else
            COFFEE_ERROR("Failed to reimport resource");
    }

    void ImportPanel::SaveSettings() {
        if (!selectedResource) return;
        
        std::filesystem::path settingsPath = selectedResource->GetPath();
        settingsPath += ".import";
        
        std::ofstream os(settingsPath, std::ios::binary);
        cereal::BinaryOutputArchive archive(os);
        archive(*m_CurrentSettings);
    }

    void ImportPanel::LoadSettings() {
        if (!selectedResource) return;
        
        std::filesystem::path settingsPath = selectedResource->GetPath();
        settingsPath += ".import";
        
        if (!std::filesystem::exists(settingsPath)) {
            // Create default settings based on resource type
            switch (selectedResource->GetType()) {
                case ResourceType::Texture2D:
                    m_CurrentSettings = std::make_unique<TextureImportSettings>();
                    break;
                case ResourceType::Model:
                    m_CurrentSettings = std::make_unique<ModelImportSettings>();
                    break;
                default:
                    break;
            }
            return;
        }
        
        std::ifstream is(settingsPath, std::ios::binary);
        cereal::BinaryInputArchive archive(is);
        archive(m_CurrentSettings);
    }

}