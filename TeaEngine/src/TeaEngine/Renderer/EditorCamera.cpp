#include "TeaEngine/Renderer/EditorCamera.h"
#include "TeaEngine/Core/Base.h"
#include "TeaEngine/Core/KeyCodes.h"
#include "TeaEngine/Core/Log.h"
#include "TeaEngine/Core/Input.h"
#include "TeaEngine/Core/MouseCodes.h"
#include "TeaEngine/Events/Event.h"
#include "TeaEngine/Events/MouseEvent.h"
#include "TeaEngine/Renderer/Camera.h"
#include <GLFW/glfw3.h>
#include <glm/ext/matrix_transform.hpp>
#include <glm/fwd.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/trigonometric.hpp>

namespace Tea {

    glm::mat4 EditorCamera::ProjectionTypeToMat4(ProjectionType projection)
    {
        switch (projection) {
            case ProjectionType::PERSPECTIVE:
                return glm::perspective(glm::radians(m_FOV), m_AspectRatio, m_NearClip, m_FarClip);
            break;
            case ProjectionType::ORTHOGRAPHIC:
                float orthoHeight = 10.0f; // Define the height of the orthographic view
                float orthoWidth = orthoHeight * m_AspectRatio; // Calculate width based on aspect ratio
                return glm::ortho(-orthoWidth / 2, orthoWidth / 2, -orthoHeight / 2, orthoHeight / 2, m_NearClip, m_FarClip);
            break;
        }
    }

    EditorCamera::EditorCamera(float fov, ProjectionType projection, float aspectRatio, float nearClip, float farClip)
        : m_FOV(fov), m_ProjectionType(projection), m_AspectRatio(aspectRatio), m_NearClip(nearClip), m_FarClip(farClip)
    {
        UpdateView();
        UpdateProjection();
    }

    void EditorCamera::OnUpdate()
    {
        glm::vec2 mousePos = Input::GetMousePosition();

        glm::vec2 delta = (mousePos - m_InitialMousePosition) * 0.01f;

        m_InitialMousePosition = mousePos;
        
        if(Input::IsMouseButtonPressed(Mouse::ButtonMiddle))
        {
            if(Input::IsKeyPressed(Key::LeftShift))
            {
                MousePan(delta);
            }
            else
            {
                MouseRotate(delta);
            }
        }

        TEA_CORE_INFO("Camera Position: ({0}, {1})", m_Position.x, m_Position.y);
        TEA_CORE_INFO("Camera Focal Point: ({0}, {1})", m_FocalPoint.x, m_FocalPoint.y);

        glm::vec3 cameraEulerAngles = glm::eulerAngles(GetOrientation());
        TEA_CORE_INFO("Camera Euler Angles: ({0}, {1}, {2})", glm::degrees(cameraEulerAngles.x), glm::degrees(cameraEulerAngles.y), glm::degrees(cameraEulerAngles.z));

        UpdateView();
    }

    void EditorCamera::OnEvent(Event& event)
    {
        EventDispatcher dispatcher(event);
        dispatcher.Dispatch<MouseScrolledEvent>(TEA_BIND_EVENT_FN(OnMouseScroll));
    }

    void EditorCamera::MouseRotate(const glm::vec2& delta)
    {
        m_Yaw += delta.x;
        m_Pitch += delta.y;
    }

    void EditorCamera::MousePan(const glm::vec2& delta)
    {
        m_FocalPoint += -GetRightDirection() * delta.x * m_Distance * 0.1f;
        m_FocalPoint += GetUpDirection() * delta.y * m_Distance * 0.1f;
    }

    void EditorCamera::MouseZoom(float delta)
    {
        m_Distance -= delta;
    }

    bool EditorCamera::OnMouseScroll(MouseScrolledEvent& event)
    {
        float delta = event.GetYOffset() * 0.5;
        MouseZoom(delta);
        UpdateView();
        return false;
    }

    void EditorCamera::UpdateProjection()
    {
        m_Projection = ProjectionTypeToMat4(m_ProjectionType);
    }

    void EditorCamera::UpdateView()
    {
        m_Position = CalculatePosition();

        m_ViewMatrix = glm::lookAt(m_Position, m_FocalPoint, GetUpDirection());
    }

    glm::vec3 EditorCamera::GetUpDirection() const
    {
        return GetOrientation() * glm::vec3(0.0f, 1.0f, 0.0f);
    }
	glm::vec3 EditorCamera::GetRightDirection() const
    {
        return GetOrientation() * glm::vec3(1.0f, 0.0f, 0.0f);
    }
	glm::vec3 EditorCamera::GetForwardDirection() const
    {
        return GetOrientation() * glm::vec3(0.0f, 0.0f, -1.0f);
    }
	glm::quat EditorCamera::GetOrientation() const
    {
        return glm::quat(glm::vec3(-m_Pitch, -m_Yaw, 0.0f));
    }

    glm::vec3 EditorCamera::CalculatePosition() const
    {
        return m_FocalPoint - GetForwardDirection() * m_Distance;
    }  

}