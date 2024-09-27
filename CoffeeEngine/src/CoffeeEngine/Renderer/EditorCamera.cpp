#include "CoffeeEngine/Renderer/EditorCamera.h"
#include "CoffeeEngine/Core/Base.h"
#include "CoffeeEngine/Core/KeyCodes.h"
#include "CoffeeEngine/Core/Input.h"
#include "CoffeeEngine/Core/MouseCodes.h"
#include "CoffeeEngine/Events/Event.h"
#include "CoffeeEngine/Events/MouseEvent.h"
#include <GLFW/glfw3.h>
#include <glm/ext/matrix_transform.hpp>
#include <glm/fwd.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/trigonometric.hpp>

namespace Coffee {

    EditorCamera::EditorCamera(float fov, ProjectionType projection, float aspectRatio, float nearClip, float farClip)
    {   
        m_FOV = fov;
        m_ProjectionType = projection;
        m_AspectRatio = aspectRatio;
        m_NearClip = nearClip;
        m_FarClip = farClip;

        UpdateView();
        UpdateProjection();
    }

    void EditorCamera::OnUpdate()
    {
        glm::vec2 mousePos = Input::GetMousePosition();

        glm::vec2 delta = (mousePos - m_InitialMousePosition) * 0.01f;

        m_InitialMousePosition = mousePos;
        
        if(Input::IsMouseButtonPressed(Mouse::BUTTON_MIDDLE))
        {
            if(Input::IsKeyPressed(Key::LSHIFT))
            {
                MousePan(delta);
            }
            else
            {
                MouseRotate(delta);
            }
        }

     /* COFFEE_CORE_INFO("Camera Position: ({0}, {1})", m_Position.x, m_Position.y);
        COFFEE_CORE_INFO("Camera Focal Point: ({0}, {1})", m_FocalPoint.x, m_FocalPoint.y);

        glm::vec3 cameraEulerAngles = glm::eulerAngles(GetOrientation());
        COFFEE_CORE_INFO("Camera Euler Angles: ({0}, {1}, {2})", glm::degrees(cameraEulerAngles.x), glm::degrees(cameraEulerAngles.y), glm::degrees(cameraEulerAngles.z));
        */
        UpdateView();
    }

    void EditorCamera::OnEvent(Event& event)
    {
        EventDispatcher dispatcher(event);
        dispatcher.Dispatch<MouseScrolledEvent>(COFFEE_BIND_EVENT_FN(OnMouseScroll));
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