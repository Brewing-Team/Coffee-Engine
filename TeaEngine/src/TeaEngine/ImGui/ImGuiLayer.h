#pragma once

#include "TeaEngine/Core/Layer.h"

namespace Tea {
    class ImGuiLayer : public Layer
    {
    public:
        ImGuiLayer();
        /// @brief 
        ~ImGuiLayer();

        void OnAttach();
        void OnDetach();
        void OnUpdate();
        void OnEvent(Event& e);

        void Begin();
		void End();

        void BlockEvents(bool block) { m_BlockEvents = block; }
    private:
    	bool m_BlockEvents = true;
    };
}