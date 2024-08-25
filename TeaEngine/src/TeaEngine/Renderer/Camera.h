#pragma once

#include <glm/ext/matrix_clip_space.hpp>
#include <glm/glm.hpp>

namespace Tea
{
    class Camera
	{
	public:
		Camera() = default;
		Camera(const glm::mat4& projection)
			: m_Projection(projection) {}

		virtual ~Camera() = default;

		const glm::mat4& GetProjection() const { return m_Projection; }
	protected:
		enum class ProjectionType
		{
			PERSPECTIVE,
			ORTHOGRAPHIC
		};

		glm::mat4 ProjectionTypeToMat4(ProjectionType projection)
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

		void UpdateProjection()
		{
			m_AspectRatio = m_ViewportWidth / m_ViewportHeight;
			m_Projection = ProjectionTypeToMat4(m_ProjectionType);
		}
	protected:
	    float m_FOV = 45.0f, m_AspectRatio = 1.778, m_NearClip = 0.1f, m_FarClip = 1000.0f;

		glm::mat4 m_Projection = glm::mat4(1.0f);

        ProjectionType m_ProjectionType = ProjectionType::PERSPECTIVE;

		float m_ViewportWidth = 1280, m_ViewportHeight = 720;
	};

}
