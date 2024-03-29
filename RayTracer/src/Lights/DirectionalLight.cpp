#include "DirectionalLight.h"
#include "glm/glm.hpp"
#include <algorithm>

namespace RayTracer
{
	DirectionalLight::DirectionalLight(glm::vec3 direction, glm::vec3 color, float intensity) : Light(color, intensity)
	{
		m_direction = glm::normalize(direction);
	}

	// Directional lighting is very straightforward
	glm::vec3 DirectionalLight::Illumination(const glm::vec3& point, const glm::vec3& normal, const glm::vec3& ray)
	{
		glm::vec3 returnIntensity = { 0,0,0 };

		float incidence = std::max(0.0f, glm::dot(normal, -m_direction));

		if (incidence > 0.0f)
		{
			returnIntensity = m_color * incidence * m_intensity;
		}

		return returnIntensity;
	}

	glm::vec3 DirectionalLight::DirectionToLight(const glm::vec3& point)
	{
		return glm::normalize(-m_direction);
	}

	float DirectionalLight::DistanceToLight(const glm::vec3 & point)
	{
		return 1000000.0f;
	}
}
