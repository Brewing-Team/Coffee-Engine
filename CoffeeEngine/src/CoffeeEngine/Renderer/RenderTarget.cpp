#include "RenderTarget.h"

namespace Coffee {

    RenderTarget::RenderTarget(const std::string& name, const glm::vec2& size, const std::vector<std::initializer_list<Attachment>>& framebufferAttachments)
        : m_Name(name), m_Size(size)
    {
        for (const auto& attachments : framebufferAttachments)
        {
            m_Framebuffers.push_back(Framebuffer::Create(size.x, size.y, attachments));
        }
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