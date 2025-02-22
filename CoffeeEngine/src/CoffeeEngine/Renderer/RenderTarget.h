#pragma once

#include "CoffeeEngine/Renderer/Camera.h"
#include "CoffeeEngine/Renderer/Framebuffer.h"
#include <string>
#include <unordered_map>
#include <vector>
#include <glm/glm.hpp>

namespace Coffee {
    
    class RenderTarget
    {
    public:
        RenderTarget() = default;
        RenderTarget(const std::string& name, const glm::vec2& size, const std::vector<std::pair<std::string, std::initializer_list<Attachment>>>& framebufferAttachments);
        ~RenderTarget() = default;

        const std::unordered_map<std::string, Ref<Framebuffer>>& GetFramebuffers() const { return m_Framebuffers; }
        const Ref<Framebuffer>& GetFramebuffer(const std::string& name) const { return m_Framebuffers.at(name); }

        const std::string& GetName() const { return m_Name; }
        void SetName(const std::string& name) { m_Name = name; }

        void SetCamera(const Camera& camera, const glm::mat4& cameraTransform)
        { 
            m_Camera = camera;
            m_Camera.SetViewportSize(m_Size.x, m_Size.y);

            m_CameraTransform = cameraTransform;
        }
        const Camera& GetCamera() const { return m_Camera; }

        const glm::mat4& GetCameraTransform() const { return m_CameraTransform; }

        void Resize(uint32_t width, uint32_t height);
    
    private:
        std::string m_Name;
        
        Camera m_Camera;
        glm::mat4 m_CameraTransform;

        glm::vec2 m_Size;
        std::unordered_map<std::string, Ref<Framebuffer>> m_Framebuffers;

        // bool m_IsActive = false;
    };

}