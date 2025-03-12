#include "ImportPanel.h"

#include "CoffeeEngine/Core/Application.h"
#include "CoffeeEngine/IO/ImportData/ImportData.h"
#include "CoffeeEngine/IO/ImportData/Texture2DImportData.h"
#include "CoffeeEngine/IO/Resource.h"
#include "CoffeeEngine/IO/ResourceLoader.h"
#include "CoffeeEngine/IO/ResourceUtils.h"
#include "CoffeeEngine/Renderer/Texture.h"
#include "imgui.h"

#include <src/EditorLayer.h>
#include <tracy/Tracy.hpp>

namespace Coffee {

    ImportPanel::ImportPanel(const Ref<Scene>& scene)
    {
    }

    void ImportPanel::OnImGuiRender()
    {
        ZoneScoped;

        if (!m_Visible) return;

        ImGui::Begin("Import");

        if (m_SelectedResource)
        {
            if (m_SelectedResource != m_LastSelectedResource)
            {
                std::filesystem::path importDataPath = m_SelectedResource->GetPath();
                importDataPath += ".import";
                m_CachedImportData = ImportDataUtils::LoadImportData(importDataPath);
                m_LastSelectedResource = m_SelectedResource;
            }

            if(m_CachedImportData)
            {
                ResourceType type = m_CachedImportData->type;
                ImGui::Text("Resource Type: %s", ResourceTypeToString(type).c_str());
            
                // Menu to Override the resource type

                switch(type)
                {
                    case ResourceType::Texture2D:
                    {
                        Texture2DImportData& textureImportData = static_cast<Texture2DImportData&>(*m_CachedImportData);
                        
                        ImGui::Checkbox("sRGB", &textureImportData.sRGB);
/*                      ImGui::Checkbox("Flip Y", &textureImportData.flipY);
                        ImGui::Checkbox("Anisotropic Filtering", &textureImportData.anisotropicFiltering);
                        ImGui::Checkbox("Generate Mipmaps", &textureImportData.generateMipmaps);
                        ImGui::SliderInt("Anisotropic Level", &textureImportData.anisotropicLevel, 1, 16);
                        ImGui::SliderInt("Wrap Mode", &textureImportData.wrapMode, 0, 2);
                        ImGui::SliderInt("Filter Mode", &textureImportData.filterMode, 0, 2); */
                        
                        if (ImGui::Button("Reimport"))
                        {
                            ImportDataUtils::SaveImportData(m_CachedImportData);
                            ResourceLoader::ReimportResource(m_SelectedResource);
                            //m_SelectedResource = ResourceLoader::Load<Texture2D>(*m_CachedImportData);
                        }

                        break;
                    }
                }
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

        ImGui::End();
    }

}