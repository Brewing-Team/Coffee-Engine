#pragma once

#include "TeaEngine/Core/Layer.h"

namespace Tea {
    class ImGuiLayer : public Layer
    {
    public:
        ImGuiLayer();
        /// @brief 
        ~ImGuiLayer();

        void OnAttach() override;
        void OnDetach() override;
        void OnEvent(Event& e) override;

        void Begin();
		void End();

        void OnImGuiRender() override;

        void BlockEvents(bool block) { m_BlockEvents = block; }

        void StyleColorsTea();
    private:
    	bool m_BlockEvents = true;
    };
}