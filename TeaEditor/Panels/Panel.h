#pragma once

#include "TeaEngine/Core/Base.h"
#include "TeaEngine/Scene/Scene.h"
#include "TeaEngine/Scene/Entity.h"

namespace Tea {

    class Panel
    {
    public:
        virtual ~Panel() = default;
        virtual void OnImGuiRender() = 0;
    };

}