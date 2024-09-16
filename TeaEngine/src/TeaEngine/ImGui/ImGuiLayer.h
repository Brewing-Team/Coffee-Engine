#pragma once

#include "TeaEngine/Core/Layer.h"

namespace Tea {

    /**
     * @defgroup imgui ImGui
     * @{
     */

    /**
     * @brief Layer for ImGui integration.
     * @ingroup imgui
     */
    class ImGuiLayer : public Layer
    {
    public:
        /**
         * @brief Constructor for ImGuiLayer.
         */
        ImGuiLayer();

        /**
         * @brief Destructor for ImGuiLayer.
         */
        ~ImGuiLayer();

        /**
         * @brief Called when the layer is attached.
         */
        void OnAttach() override;

        /**
         * @brief Called when the layer is detached.
         */
        void OnDetach() override;

        /**
         * @brief Called when an event occurs.
         * @param e The event.
         */
        void OnEvent(Event& e) override;

        /**
         * @brief Begin ImGui frame.
         */
        void Begin();

        /**
         * @brief End ImGui frame.
         */
        void End();

        /**
         * @brief Called to render ImGui.
         */
        void OnImGuiRender() override;

        /**
         * @brief Block or unblock events.
         * @param block Whether to block events.
         */
        void BlockEvents(bool block) { m_BlockEvents = block; }

        /**
         * @brief Set the Tea color style for ImGui.
         */
        void SetTeaColorStyle();

        /**
         * @brief Set the Godot color style for ImGui.
         */
        void SetGodotColorStyle();

    private:
        bool m_BlockEvents = true;
    };

    /** @} */ // end of imgui group
}
