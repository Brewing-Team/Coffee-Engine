#pragma once

#include "CoffeeEngine/Renderer/Camera.h"
#include "CoffeeEngine/Renderer/Framebuffer.h"
#include <string>
#include <vector>
#include <glm/glm.hpp>

namespace Coffee {
    
    class RenderTarget
    {
    public:
        RenderTarget() = default;
        RenderTarget(const std::string& name, const glm::vec2& size, const std::vector<std::initializer_list<Attachment>>& framebufferAttachments)
            : m_Name(name), m_Size(size)
        {
            for (const auto& attachments : framebufferAttachments)
            {
                m_Framebuffers.push_back(Framebuffer::Create(size.x, size.y, attachments));
            }
        }
        ~RenderTarget() = default;

        const std::vector<Ref<Framebuffer>>& GetFramebuffers() const { return m_Framebuffers; }
        const Ref<Framebuffer>& GetFramebuffer(const std::string& name) const;

        const std::string& GetName() const { return m_Name; }
        void SetName(const std::string& name) { m_Name = name; }
    
    private:
        std::string m_Name;

        // Camera?

        glm::vec2 m_Size;
        std::vector<Ref<Framebuffer>> m_Framebuffers;

        // bool m_IsActive = false;
    };

}