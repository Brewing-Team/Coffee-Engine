#pragma once

#include "TeaEngine/Events/Event.h"
#include "TeaEngine/Events/MouseEvent.h"
#include <TeaEngine/Renderer/Camera.h>

#include <glm/fwd.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace Tea {

    enum class ProjectionType
    {
        PERSPECTIVE,
        ORTHOGRAPHIC
    };

    class EditorCamera : public Camera
    {
    public:
        EditorCamera() = default;
        EditorCamera(float fov, ProjectionType projection = ProjectionType::PERSPECTIVE, float aspectRatio = 1.778, float nearClip = 0.1f, float farClip = 1000.0f);

        void OnUpdate();
        void OnEvent(Event& event);

        inline void SetViewportSize(float width, float height) { m_ViewportWidth = width; m_ViewportHeight = height; UpdateProjection(); }

        inline void SetFocusPoint(glm::vec3 focusPoint) { m_FocalPoint = focusPoint; }

        const glm::mat4 GetViewMatrix() const { return m_ViewMatrix; };

        glm::vec3 GetUpDirection() const;
		glm::vec3 GetRightDirection() const;
		glm::vec3 GetForwardDirection() const;
		const glm::vec3& GetPosition() const { return m_Position; }
		glm::quat GetOrientation() const;

    private:
        glm::mat4 ProjectionTypeToMat4(ProjectionType projection);

        void UpdateProjection();
        void UpdateView();

        bool OnMouseScroll(MouseScrolledEvent& event);

        glm::vec3 CalculatePosition() const;

		void MouseRotate(const glm::vec2& delta);
        void MousePan(const glm::vec2& delta);
        void MouseZoom(float delta);

    private:
        float m_FOV = 45.0f, m_AspectRatio = 1.778, m_NearClip = 0.1f, m_FarClip = 1000.0f;

        ProjectionType m_ProjectionType = ProjectionType::PERSPECTIVE;

        glm::mat4 m_ViewMatrix;

        glm::vec3 m_Position = glm::vec3(0.0f, 0.0f, 0.0f);
        glm::vec3 m_FocalPoint = glm::vec3(0.0f, 0.0f, 0.0f);

        float m_Distance = 10.0f;

        float m_Pitch = 0.0f, m_Yaw = 0.0f;

        glm::vec2 m_InitialMousePosition = glm::vec2(0.0f, 0.0f);

        float m_ViewportWidth = 1280, m_ViewportHeight = 720;
    };
}