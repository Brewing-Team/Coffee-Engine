#pragma once

#include "CoffeeEngine/Core/Base.h"
#include "Panel.h"

#include <filesystem>

namespace Coffee {

    class Scene;
    class Resource;
}

namespace Coffee {

    class ContentBrowserPanel : public Panel
    {
    public:
        ContentBrowserPanel() = default;
        ContentBrowserPanel(const Ref<Scene>& scene);

        void SetContext(const Ref<Scene>& scene);

        const Ref<Resource>& GetSelectedResource() const { return m_SelectedResource; }

        void OnImGuiRender() override;

    private:
        void DisplayDirectoryContents(const std::filesystem::path& directory, int depth);

    private:
        std::filesystem::path m_CurrentDirectory;
        std::filesystem::path m_SelectedDirectory;
        Ref<Scene> m_Context;
        Ref<Resource> m_SelectedResource;
    };

}