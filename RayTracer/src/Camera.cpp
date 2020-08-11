#include "Camera.h"

namespace RayTracer
{
	Camera::Camera(unsigned int width, unsigned int height, float fov, glm::vec3 position, glm::vec3 lookAt, glm::vec3 upVector)
		: m_width(width), m_height(height), m_fov(fov), m_position(position), m_lookAt(lookAt), m_upVector(upVector)
	{
	}
}