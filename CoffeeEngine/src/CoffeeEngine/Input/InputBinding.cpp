#include "CoffeeEngine/Input/InputBinding.h"

#include "CoffeeEngine/Core/Input.h"

namespace Coffee {

    float InputBinding::GetValue(ControllerCode controller) const // ASK about the unused controller parameter
    {
        if (IsAnalog && Axis != Axis::Invalid)
        {
            return Input::GetAxisRaw(Axis);
        }

        float value = 0.0f;
        if (Input::IsKeyPressed(KeyPos) || Input::GetButtonRaw(ButtonPos))
            value += 1.0f;
        if (Input::IsKeyPressed(KeyNeg) || Input::GetButtonRaw(ButtonNeg))
            value -= 1.0f;

        return value;
    }

}
