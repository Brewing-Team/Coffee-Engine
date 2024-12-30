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

            // TODO remove temp mockup variables
            bool srgb = false;
            bool flipY = false;
            bool flipX = false;
            float globalScale;
            bool ignoreLight = false;
            bool ignoreCamera = false;
    };

}