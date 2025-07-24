#pragma once

#include <string>

namespace Coffee {

    class Event;

}

namespace Coffee {
    /**
     * @defgroup core Core
     * @{
     */
	class Layer
{
public:
    /**
     * Constructor for the Layer class.
     * @param name The name of the layer, default is "Layer".
     */
    Layer(const std::string& name = "Layer") : m_DebugName(name) {}

    /**
     * Virtual destructor for the Layer class.
     */
    virtual ~Layer() = default;

    /**
     * Called when the layer is attached.
     */
    virtual void OnAttach() {}

    /**
     * Called when the layer is detached.
     */
    virtual void OnDetach() {}

    /**
     * Called every frame to update the layer.
     * @param dt Delta time since the last frame.
     */
    virtual void OnUpdate(float dt) {}

    /**
     * Called to render ImGui elements.
     */
    virtual void OnImGuiRender() {}

    /**
     * Called when an event occurs.
     * @param event The event that occurred.
     */
    virtual void OnEvent(Event& event) {}

    /**
     * Gets the name of the layer.
     * @return The name of the layer.
     */
    const std::string& GetName() const { return m_DebugName; }

protected:
    std::string m_DebugName;
};
    /** @} */
}