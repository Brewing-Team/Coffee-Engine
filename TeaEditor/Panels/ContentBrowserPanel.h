#pragma once

#include "TeaEngine/Core/Base.h"
#include "TeaEngine/Scene/Scene.h"
#include "TeaEngine/Scene/Entity.h"
#include "Panel.h"

namespace Tea {

    class ContentBrowserPanel : public Panel
    {
    public:
        ContentBrowserPanel() = default;
        ContentBrowserPanel(const Ref<Scene>& scene);

        void OnImGuiRender() override;
    
    private:
        void DisplayDirectoryContents(const std::filesystem::path& directory, int depth);
    private:
        std::filesystem::path m_CurrentDirectory;

        std::filesystem::path m_SelectedDirectory;
    };

}