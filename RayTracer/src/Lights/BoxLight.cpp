#include "BoxLight.h"

#include <cstdlib>
#include <ctime>
#include <algorithm>

namespace RayTracer
{
	RectLight::RectLight(glm::mat4 transform, glm::vec3 dimensions, ConstMaterial * material, glm::vec3 color, float intensity)
		: AreaLight(transform, material, color, intensity), m_dimensions(dimensions)
	{
		float xDim = dimensions.x / 2.0f;
		float yDim = dimensions.y / 2.0f;
		float zDim = dimensions.z / 2.0f;

		m_aabb.minDimensions = { -xDim, -yDim, -zDim };
		m_aabb.maxDimensions = { xDim, yDim, zDim };
	}

	glm::vec3 RectLight::SampleRadiance(const glm::vec3& point, const glm::vec3& normal, glm::vec3 &sampleDirection, float &pdf, float &sampleDistance)
	{
		glm::vec3 returnRadiance = { 0,0,0 };

		glm::vec3 samplePoint;

		float incidence = std::max(0.0f, glm::dot(glm::normalize(normal), sampleDirection));

		if (incidence == 0.0f) return returnRadiance;

		sampleDistance = glm::distance(samplePoint, point);
		float lightReceived = m_intensity / (sampleDistance * sampleDistance);
		returnRadiance = (m_color * lightReceived * incidence);

		return returnRadiance;
	}
}