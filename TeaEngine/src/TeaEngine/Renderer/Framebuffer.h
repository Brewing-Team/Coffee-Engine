#pragma once

#include "TeaEngine/Renderer/Texture.h"
#include <cstdint>
#include <initializer_list>
#include <sys/types.h>
#include <vector>
namespace Tea {

    class Framebuffer
    {
    public:
        Framebuffer(uint32_t width, uint32_t height, std::initializer_list<ImageFormat> attachments);
        ~Framebuffer();

        void Invalidate();

        void Bind();
        void UnBind();

        void Resize(uint32_t width, uint32_t height);

        void AttachColorTexture(Texture* texture);
        void AttachDepthTexture(Texture* texture);

        uint32_t GetColorAttachmentID (uint32_t index = 0) const { return m_ColorTextures[index]->GetID(); }
        
        static Ref<Framebuffer> Create(uint32_t width, uint32_t height, std::initializer_list<ImageFormat> attachments);

    private:
        uint32_t m_fboID;

        uint32_t m_Width;
        uint32_t m_Height;

        std::vector<ImageFormat> m_Attachments;

        std::vector<Texture*> m_ColorTextures = {};
        Texture* m_DepthTexture = nullptr;
    };

}