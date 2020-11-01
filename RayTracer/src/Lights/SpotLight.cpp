#include "SpotLight.h"
#include <algorithm>

namespace RayTracer
{
	SpotLight::SpotLight(glm::vec3 color, float intensity, glm::vec3 position, glm::vec3 direction, float innerAngle, float outerAngle) : Light(color, intensity), m_position(position)
	{
		m_direction = glm::normalize(direction);
		m_innerAngle = glm::cos(glm::radians(innerAngle));
		if (outerAngle == 0.0f) m_outerAngle = m_innerAngle;
		else m_outerAngle = glm::cos(glm::radians(outerAngle));
	}

	// This code was modeled after spotlights on learnopengl.com for the sake of having a "soft" edge
	glm::vec3 SpotLight::SampleRadiance(const glm::vec3& point, const glm::vec3& normal, glm::vec3 &sampleDirection, float &pdf, float &sampleDistance)
	{
		glm::vec3 returnRadiance = { 0,0,0 };

		sampleDirection = glm::normalize(m_position - point);
		pdf = 1.0f;

		float theta = glm::dot(sampleDirection, -m_direction);

		if (theta < m_outerAngle) return returnRadiance;

		float incidence = std::max(0.0f, glm::dot(normal, sampleDirection));

		if (incidence == 0.0f) return returnRadiance;

		float epsilon = (m_innerAngle - m_outerAngle);
		float intensity = glm::clamp((theta - m_outerAngle) / epsilon, 0.0f, 1.0f);
		sampleDistance = glm::distance(m_position, point);
		float lightReceived = m_intensity / (sampleDistance * sampleDistance);

		returnRadiance = m_color * lightReceived * intensity * incidence;

		return returnRadiance;
	}
}