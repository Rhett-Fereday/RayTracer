#include "SpotLight.h"
#include <algorithm>

namespace RayTracer
{
	SpotLight::SpotLight(glm::vec3 color, float intensity, glm::vec3 position, glm::vec3 direction, float angle) : Light(color, intensity), m_position(position), m_direction(direction)
	{
		m_angle = glm::cos(glm::radians(angle));
	}

	glm::vec3 SpotLight::Illumination(const glm::vec3 & point, const glm::vec3 & normal, const glm::vec3 & ray)
	{
		glm::vec3 returnIntensity = { 0,0,0 };

		float angleBetween = glm::dot(DirectionToLight(point) , -m_direction);

		if (angleBetween > m_angle)
		{

			float incidence = std::max(0.0f, glm::dot(normal, DirectionToLight(point)));

			if (incidence > 0.0f)
			{
				float distance = glm::distance(m_position, point);
				float lightReceived = m_intensity / (distance * distance);
				returnIntensity = m_color * lightReceived * incidence;
			}
		}

		return returnIntensity;
	}

	glm::vec3 SpotLight::DirectionToLight(const glm::vec3 & point)
	{
		return glm::normalize(m_position - point);
	}

	float SpotLight::DistanceToLight(const glm::vec3 & point)
	{
		return glm::distance(m_position, point);
	}
}