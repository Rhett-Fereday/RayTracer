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
	glm::vec3 DirectionalLight::SampleRadiance(const glm::vec3& point, const glm::vec3& normal, glm::vec3 &sampleDirection, float &pdf, float &sampleDistance)
	{
		glm::vec3 returnRadiance = { 0,0,0 };
		
		float incidence = std::max(0.0f, glm::dot(normal, -m_direction));

		if (incidence == 0.0f) return returnRadiance;

		returnRadiance = m_color * m_intensity * incidence;

		sampleDirection = -m_direction;
		pdf = 1.0f;
		sampleDistance = INFINITY;

		return returnRadiance;
	}
}
