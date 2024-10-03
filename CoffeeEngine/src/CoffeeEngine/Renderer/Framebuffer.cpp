#include "Framebuffer.h"
#include "CoffeeEngine/Core/Base.h"
#include "CoffeeEngine/Renderer/Texture.h"

#include <cstdint>
#include <glad/glad.h>
#include <tracy/Tracy.hpp>

#include <glm/vec4.hpp>

namespace Coffee {

    static const uint32_t s_MaxFramebufferSize = 8192;

    Framebuffer::Framebuffer(uint32_t width, uint32_t height, std::initializer_list<ImageFormat> attachments)
        : m_Width(width), m_Height(height), m_Attachments(attachments)
    {
        ZoneScoped;

        glCreateFramebuffers(1, &m_fboID);

        Invalidate();
    }

    Framebuffer::~Framebuffer()
    {
        glDeleteFramebuffers(1, &m_fboID);
    }

    void Framebuffer::Resize(uint32_t width, uint32_t height)
    {
        ZoneScoped;

        if(width == 0 || height == 0 || width > s_MaxFramebufferSize || height > s_MaxFramebufferSize)
        {
            COFFEE_CORE_WARN("Attempted to resize framebuffer to {0}, {1}", width, height);
            return;
        }

        m_Width = width;
        m_Height = height;

        /* for (auto& texture : m_ColorTextures)
        {
            texture->SetData(nullptr, m_Width * m_Height * 4);
        }

        m_DepthTexture->SetData(nullptr, m_Width * m_Height * 4); */

        Invalidate();
    }

    void Framebuffer::Invalidate()
    {
        ZoneScoped;

        if(m_fboID)
        {
            glDeleteFramebuffers(1, &m_fboID);

            //m_ColorTextures.clear();
            //m_DepthTexture.reset();

            glCreateFramebuffers(1, &m_fboID);
            glBindFramebuffer(GL_FRAMEBUFFER, m_fboID);

            for (size_t i = 0; i < m_Attachments.size(); i++)
            {
                ImageFormat imageFormat = m_Attachments[i];

                if(imageFormat == ImageFormat::DEPTH24STENCIL8)
                {
                    if(m_DepthTexture)
                    {
                        m_DepthTexture->Resize(m_Width, m_Height);
                        glNamedFramebufferTexture(m_fboID, GL_DEPTH_STENCIL_ATTACHMENT, m_DepthTexture->GetID(), 0);
                        continue;
                    }
                    else
                    {
                        Ref<Texture> depthTexture = Texture::Create(m_Width, m_Height, imageFormat);
                        m_DepthTexture = depthTexture;
                        glNamedFramebufferTexture(m_fboID, GL_DEPTH_STENCIL_ATTACHMENT, depthTexture->GetID(), 0);
                    }
                }
                else
                {
                    if(m_ColorTextures.size() > i)
                    {
                        m_ColorTextures[i]->Resize(m_Width, m_Height);
                        glNamedFramebufferTexture(m_fboID, GL_COLOR_ATTACHMENT0 + i, m_ColorTextures[i]->GetID(), 0);
                        continue;
                    }
                    else
                    {
                        Ref<Texture> colorTexture = Texture::Create(m_Width, m_Height, imageFormat);
                        m_ColorTextures.push_back(colorTexture);
                        glNamedFramebufferTexture(m_fboID, GL_COLOR_ATTACHMENT0 + m_ColorTextures.size() - 1, colorTexture->GetID(), 0);
                    }
                }
            }
            //Set all the render buffers to be drawn to (Wrap it in a function)
            std::vector<GLenum> drawBuffers;
            for (size_t i = 0; i < m_ColorTextures.size(); ++i)
            {
                drawBuffers.push_back(GL_COLOR_ATTACHMENT0 + i);
            }

            glNamedFramebufferDrawBuffers(m_fboID, drawBuffers.size(), drawBuffers.data());

            glBindFramebuffer(GL_FRAMEBUFFER, 0);
        }
    }

    void Framebuffer::Bind()
    {
        ZoneScoped;

        glBindFramebuffer(GL_FRAMEBUFFER, m_fboID);
        glViewport(0, 0, m_Width, m_Height);
    }

    void Framebuffer::UnBind()
    {
        ZoneScoped;

        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }

    glm::vec4 Framebuffer::GetPixelColor(int x, int y, uint32_t attachmentIndex)
    {
        ZoneScoped;

        COFFEE_CORE_ASSERT(attachmentIndex < m_ColorTextures.size(), "Attachment index out of bounds");

        glBindFramebuffer(GL_FRAMEBUFFER, m_fboID);
        glReadBuffer(GL_COLOR_ATTACHMENT0 + attachmentIndex);

        glm::vec4 result;
        glReadPixels(x, y, 1, 1, GL_RGBA, GL_FLOAT, &result);

        glBindFramebuffer(GL_FRAMEBUFFER, 0);

        return result;
    }

    void Framebuffer::SetDrawBuffers(std::initializer_list<Ref<Texture>> colorAttachments)
    {
        ZoneScoped;

        //TODO: Improve this code, double for loop is not efficient at all a map would be better i think
        std::vector<GLenum> drawBuffers;
        for (int i = 0; i < colorAttachments.size(); i++)
        {
            for (int j = 0; j < m_ColorTextures.size(); j++)
            {
                if(colorAttachments.begin()[i]->GetID() == m_ColorTextures[j]->GetID())
                {
                    drawBuffers.push_back(GL_COLOR_ATTACHMENT0 + j);
                    break;
                }
            }
        }

        glNamedFramebufferDrawBuffers(m_fboID, drawBuffers.size(), drawBuffers.data());
    }

    void Framebuffer::SetDrawBuffers(std::initializer_list<uint32_t> colorAttachments)
    {
        ZoneScoped;

        std::vector<GLenum> drawBuffers;
        for (int i = 0; i < colorAttachments.size(); i++)
        {
            drawBuffers.push_back(GL_COLOR_ATTACHMENT0 + colorAttachments.begin()[i]);
        }

        glNamedFramebufferDrawBuffers(m_fboID, drawBuffers.size(), drawBuffers.data());
    }

    void Framebuffer::AttachColorTexture(Ref<Texture>& texture)
    {
        ZoneScoped;

        m_ColorTextures.push_back(texture);
        m_Attachments.push_back(texture->GetImageFormat());
        glNamedFramebufferTexture(m_fboID, GL_COLOR_ATTACHMENT0 + m_ColorTextures.size() - 1, texture->GetID(), 0);
    }

    void Framebuffer::AttachDepthTexture(Ref<Texture>& texture)
    {
        ZoneScoped;

        m_DepthTexture = texture;
        glNamedFramebufferTexture(m_fboID, GL_DEPTH_STENCIL_ATTACHMENT, texture->GetID(), 0);
    }

    Ref<Framebuffer> Framebuffer::Create(uint32_t width, uint32_t height, std::initializer_list<ImageFormat> attachments)
    {
        return CreateRef<Framebuffer>(width, height, attachments);
    }

}
