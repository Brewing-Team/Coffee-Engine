#pragma once

#include "TeaEngine/Core/Base.h"
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

        const uint32_t GetWidth() const { return m_Width; }
        const uint32_t GetHeight() const { return m_Height; }

        void AttachColorTexture(Ref<Texture>& texture);
        void AttachDepthTexture(Ref<Texture>& texture);

        uint32_t GetColorAttachmentID (uint32_t index = 0) const { return m_ColorTextures[index]->GetID(); }
        
        static Ref<Framebuffer> Create(uint32_t width, uint32_t height, std::initializer_list<ImageFormat> attachments);

    private:
        uint32_t m_fboID;

        uint32_t m_Width;
        uint32_t m_Height;

        std::vector<ImageFormat> m_Attachments;

        std::vector<Ref<Texture>> m_ColorTextures;
        Ref<Texture> m_DepthTexture;
    };

}