#pragma once

#include "CoffeeEngine/Core/Base.h"
#include "CoffeeEngine/Scene/Scene.h"
#include "CoffeeEngine/Scene/Entity.h"
#include "Panel.h"

namespace Coffee {

    class ContentBrowserPanel : public Panel
    {
    public:
        ContentBrowserPanel() = default;
        ContentBrowserPanel(const Ref<Scene>& scene);

        void SetContext(const Ref<Scene>& scene);

        void OnImGuiRender() override;
    
    private:
        void DisplayDirectoryContents(const std::filesystem::path& directory, int depth);
    private:
        std::filesystem::path m_CurrentDirectory;

        std::filesystem::path m_SelectedDirectory;

        Ref<Scene> m_Context;
    };

}