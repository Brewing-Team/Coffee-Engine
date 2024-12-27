#pragma once

#include "Panel.h"

namespace Coffee {

    class ImportPanel : public Panel
    {
        public:
            ImportPanel() = default;
            ImportPanel(const Ref<Scene>& scene);

            void SetContext(const Ref<Scene>& scene);

            void OnImGuiRender() override;

            Ref<Resource> selectedResource;

        private:
            Ref<Scene> m_Context;
    };

}