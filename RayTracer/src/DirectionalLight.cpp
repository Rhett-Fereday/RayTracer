#include "DirectionalLight.h"
#include "glm/glm.hpp"
#include <algorithm>

namespace RayTracer
{
	DirectionalLight::DirectionalLight(glm::vec3 direction, glm::vec3 color, float intensity) : m_color(color), m_intensity(intensity)
	{
		m_direction = glm::normalize(direction);
	}

	glm::vec3 DirectionalLight::illumination(const glm::vec3& point, const glm::vec3& normal, const glm::vec3& ray)
	{
		glm::vec3 returnIntensity = { 0,0,0 };

		float incidence = std::max(0.0f, glm::dot(normal, m_direction));

		if (incidence > 0.0f)
		{
			returnIntensity = m_color * incidence;
		}

		return returnIntensity;
	}
}
