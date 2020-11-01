#include "PointLight.h"
#include <algorithm>


namespace RayTracer
{
	PointLight::PointLight(glm::vec3 color, float intensity, glm::vec3 position) : Light(color, intensity), m_position(position)
	{
	}

	// Point light illumination is also very easy. Spherical attenuation from scratchapixel.com
	glm::vec3 PointLight::SampleRadiance(const glm::vec3& point, const glm::vec3& normal, glm::vec3 &sampleDirection, float &pdf, float &sampleDistance)
	{
		glm::vec3 returnRadiance = { 0,0,0 };

		sampleDirection = glm::normalize(m_position - point);
		pdf = 1.0f;

		float incidence = std::max(0.0f, glm::dot(normal, sampleDirection));

		if (incidence == 0.0f) return returnRadiance;

		sampleDistance = glm::distance(m_position, point);
		float lightReceived = m_intensity / (4.0f * 3.14f * (sampleDistance * sampleDistance));
		returnRadiance = m_color * lightReceived * incidence;

		return returnRadiance;
	}
}