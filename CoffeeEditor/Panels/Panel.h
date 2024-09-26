#pragma once

#include "CoffeeEngine/Core/Base.h"
#include "CoffeeEngine/Scene/Scene.h"
#include "CoffeeEngine/Scene/Entity.h"

namespace Coffee {

    class Panel
    {
    public:
        virtual ~Panel() = default;
        virtual void OnImGuiRender() = 0;
    };

}