#pragma once

#include "TeaEngine/Core/Layer.h"

namespace Tea {

    /**
     * @defgroup imgui ImGui
     * @brief ImGui components of the TeaEngine.
     * @{
     */

    /**
     * @brief The ImGuiLayer class is responsible for managing the ImGui layer in the application.
     */
    class ImGuiLayer : public Layer
    {
    public:
        /**
         * @brief Constructs the ImGuiLayer object.
         */
        ImGuiLayer();

        /**
         * @brief Destroys the ImGuiLayer object.
         */
        ~ImGuiLayer();

        /**
         * @brief Attaches the ImGui layer to the application.
         */
        void OnAttach() override;

        /**
         * @brief Detaches the ImGui layer from the application.
         */
        void OnDetach() override;

        /**
         * @brief Handles events for the ImGui layer.
         * @param e The event to handle.
         */
        void OnEvent(Event& e) override;

        /**
         * @brief Begins a new ImGui frame.
         */
        void Begin();

        /**
         * @brief Ends the current ImGui frame.
         */
        void End();

        /**
         * @brief Renders ImGui elements.
         */
        void OnImGuiRender() override;

        /**
         * @brief Blocks or unblocks events from being handled by the ImGui layer.
         * @param block True to block events, false to unblock.
         */
        void BlockEvents(bool block) { m_BlockEvents = block; }

        /**
         * @brief Sets the Tea color style for ImGui.
         */
        void SetTeaColorStyle();

        /**
         * @brief Sets the Godot color style for ImGui.
         */
        void SetGodotColorStyle();

    private:
        bool m_BlockEvents = true; ///< Indicates whether events are blocked.
    };

    /** @} */
}