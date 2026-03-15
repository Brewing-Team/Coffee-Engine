#pragma once

#include "Panel.h"
#include "CoffeeEngine/Core/Base.h"
#include "CoffeeEngine/Core/EngineContext.h"

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

        void SetEngineContext(const EngineContext& context) { m_EngineContext = &context; }

        void OnImGuiRender() override;
        void SetSelectedResource(const Ref<Resource>& resource) { m_SelectedResource = resource; }
    private:
        Ref<Resource> m_SelectedResource; // Think if this should be a Ref
        Ref<Resource> m_LastSelectedResource;
        Scope<ImportData> m_CachedImportData;
        const EngineContext* m_EngineContext = nullptr;
    };

}