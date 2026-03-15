#include "CoffeeEngine/Input/InputBinding.h"

#include "CoffeeEngine/Core/Input.h"

namespace Coffee {

    float InputBinding::AsAxis(bool digital) const
    {
        if (!m_Input)
            return 0.0f;

        float value = m_Input->GetAxisRaw(Axis);
        if (invertedAxis)
            value = -value;

        value += (m_Input->IsKeyPressed(KeyPos) || m_Input->GetButtonRaw(ButtonPos));
        value -= (m_Input->IsKeyPressed(KeyNeg) || m_Input->GetButtonRaw(ButtonNeg));

        value = glm::clamp(value, -1.0f, 1.0f);

        if (digital)
            return glm::round(value);
        else
            return value;
    }
    bool InputBinding::AsBool()
    {
        if (!m_Input)
            return false;

        bool value = glm::abs(m_Input->GetAxisRaw(Axis)) != 0.0f;
        value |= m_Input->GetButtonRaw(ButtonPos);
        value |= m_Input->IsKeyPressed(KeyPos);

        // "Negative" buttons and keys used as alternatives to the main button and key
        value |= m_Input->GetButtonRaw(ButtonNeg);
        value |= m_Input->IsKeyPressed(KeyNeg);

        return value;
    }

    ButtonState InputBinding::AsButton()
    {
        if (!m_Input)
            return m_State;

        // If already called this update, return cached value, otherwise recalculate state and update timestamp
        if (latestUpdate >= m_Input->m_Timestamp) return m_State;
        latestUpdate = m_Input->m_Timestamp;

        bool value = this->AsBool();

        // Set button state
        // Pressed button while Not Down or Repeat -> Down
        // Pressed button while Down -> Repeat
        // Release button while not Idle or Up -> Up
        // Button released while Up -> Idle
        switch (m_State)
        {
            using namespace ButtonStates;
            case IDLE: {
                if (value) m_State = DOWN;
                break;
            }
            case DOWN: {
                if (value) m_State = REPEAT;
                else m_State = UP;
                break;
            }
            case REPEAT: {
                if (!value) m_State = UP;
                break;
            }
            case UP: {
                if (value) m_State = DOWN;
                break;
            }
        }

        return m_State;
    }

} // namespace Coffee
