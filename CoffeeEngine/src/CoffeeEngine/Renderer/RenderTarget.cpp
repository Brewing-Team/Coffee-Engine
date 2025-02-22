#include "RenderTarget.h"

namespace Coffee {

    RenderTarget::RenderTarget(const std::string& name, const glm::vec2& size, const std::vector<std::pair<std::string, std::initializer_list<Attachment>>>& framebufferAttachments)
        : m_Name(name), m_Size(size)
    {
        for (const auto& attachments : framebufferAttachments)
        {
            m_Framebuffers[attachments.first] = Framebuffer::Create(size.x, size.y, attachments.second);
        }
    }

    void RenderTarget::Resize(uint32_t width, uint32_t height)
    {
        m_Size = {width, height};

        for (auto& [name, framebuffer] : m_Framebuffers)
        {
            framebuffer->Resize(width, height);
        }
    }

}