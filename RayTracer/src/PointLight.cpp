#include "PointLight.h"
#include <algorithm>


namespace RayTracer
{
	PointLight::PointLight(glm::vec3 color, float intensity, glm::vec3 position) : Light(color, intensity), m_position(position)
	{
	}

	glm::vec3 PointLight::Illumination(const glm::vec3 & point, const glm::vec3 & normal, const glm::vec3 & ray)
	{
		glm::vec3 returnIntensity = { 0,0,0 };

		float incidence = std::max(0.0f, glm::dot(normal, DirectionToLight(point)));

		if (incidence > 0.0f)
		{
			float r = glm::distance(m_position, point);
			float lightReceived = m_intensity / (4.0f * 3.14f * (r * r));
			returnIntensity = m_color * lightReceived * incidence;
		}

		return returnIntensity;
	}

	glm::vec3 PointLight::DirectionToLight(const glm::vec3 & point)
	{
		return glm::normalize(m_position - point);
	}

	float PointLight::DistanceToLight(const glm::vec3 & point)
	{
		float dist = glm::distance(m_position, point);
		return dist;
	}
}