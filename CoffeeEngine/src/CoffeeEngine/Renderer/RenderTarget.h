#pragma once

#include "CoffeeEngine/Renderer/Camera.h"
#include "CoffeeEngine/Renderer/Framebuffer.h"
#include <string>
#include <vector>
#include <glm/glm.hpp>

namespace Coffee {
    
    struct CameraData
    {
        glm::mat4 projection; ///< The projection matrix.
        glm::mat4 view; ///< The view matrix.
        glm::vec3 position; ///< The position of the camera.
    };

    class RenderTarget
    {
    public:
        RenderTarget() = default;
        RenderTarget(const std::string& name, const glm::vec2& size, const std::vector<std::initializer_list<Attachment>>& framebufferAttachments);
        ~RenderTarget() = default;

        const std::vector<Ref<Framebuffer>>& GetFramebuffers() const { return m_Framebuffers; }
        const Ref<Framebuffer>& GetFramebuffer(const std::string& name) const;

        const std::string& GetName() const { return m_Name; }
        void SetName(const std::string& name) { m_Name = name; }

        void SetCameraData(const CameraData& cameraData) { m_CameraData = cameraData; }
        const CameraData& GetCameraData() const { return m_CameraData; }
    
    private:
        std::string m_Name;

        CameraData m_CameraData;

        glm::vec2 m_Size;
        std::vector<Ref<Framebuffer>> m_Framebuffers;

        // bool m_IsActive = false;
    };

}