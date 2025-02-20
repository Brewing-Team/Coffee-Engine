#include "RenderTarget.h"

namespace Coffee {

    RenderTarget::RenderTarget(const std::string& name, const glm::vec2& size)
        : m_Name(name), m_Size(size)
    {
    }

    const Ref<Framebuffer>& RenderTarget::GetFramebuffer(const std::string& name) const
    {
        for (auto& framebuffer : m_Framebuffers)
        {
            if (framebuffer->GetName() == name)
            {
                return framebuffer;
            }
        }

        return nullptr;
    }

}