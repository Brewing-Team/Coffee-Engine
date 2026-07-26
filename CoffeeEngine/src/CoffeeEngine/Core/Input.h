#pragma once

#include "CoffeeEngine/Core/ControllerCodes.h"
#include "CoffeeEngine/Core/KeyCodes.h"
#include "CoffeeEngine/Core/KeyState.h"
#include "CoffeeEngine/Core/MouseCodes.h"
#include "CoffeeEngine/Events/KeyEvent.h"
#include "CoffeeEngine/Events/MouseEvent.h"
#include "MouseMode.h"
#include "Timer.h"

#include "CoffeeEngine/Events/Event.h"

#include <glm/glm.hpp>
#include <unordered_map>

namespace Coffee
{
class Window;
}

namespace Coffee
{

/**
 * @defgroup core Core
 * @brief Core components of the CoffeeEngine.
 * @{
 */

class Input
{
  public:
    /**
     * Initializes the module
     */
    Input(const Window& window);

    void Update();

    bool IsKeyJustPressed(const KeyCode key);
    bool IsKeyPressed(const KeyCode key);
    bool IsKeyReleased(const KeyCode key);

    bool IsMouseButtonJustPressed(const MouseCode button);
    bool IsMouseButtonPressed(const MouseCode button);
    bool IsMouseButtonReleased(const MouseCode button);

    void SetMouseMode(const Window& window, MouseMode mouseMode);

    /**
     * Retrieves the current position of the mouse.
     *
     * @return The current position of the mouse as a 2D vector.
     */
    const glm::vec2& GetMousePosition();
    glm::vec2 GetMouseDelta();

private:
    const Window& m_Window;

    std::array<KeyState, Key::Count> m_keys;
    std::array<KeyState, Mouse::COUNT> m_mouseButtons;
    glm::vec2 m_mousePosition;
    glm::vec2 m_mouseWheelDelta;
    MouseMode m_mouseMode;

};
/** @} */
} // namespace Coffee