#pragma once

#include "Panel.h"
#include "CoffeeEngine/Core/Base.h"

namespace Coffee {

    class Scene;
    class Resource;
    class ImportData;
}

namespace Coffee {

    class ImportPanel : public Panel
    {
    public:
        ImportPanel() = default;
        ImportPanel(const Ref<Scene>& scene);

        void OnImGuiRender() override;
        void SetSelectedResource(const Ref<Resource>& resource) { m_SelectedResource = resource; }
    private:
        Ref<Resource> m_SelectedResource; // Think if this should be a Ref
        Ref<Resource> m_LastSelectedResource;
        Scope<ImportData> m_CachedImportData;
    };

}