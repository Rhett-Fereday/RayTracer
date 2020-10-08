#include "BoxLight.h"

#include <cstdlib>
#include <ctime>
#include <algorithm>

namespace RayTracer
{
	BoxLight::BoxLight(glm::mat4 transform, glm::vec3 dimensions, ConstMaterial * material, glm::vec3 color, float intensity)
		: AreaLight(transform, material, color, intensity), m_dimensions(dimensions)
	{
		float xDim = dimensions.x / 2.0f;
		float yDim = dimensions.y / 2.0f;
		float zDim = dimensions.z / 2.0f;

		m_aabb.minDimensions = { -xDim, -yDim, -zDim };
		m_aabb.maxDimensions = { xDim, yDim, zDim };
	}

	bool BoxLight::TestIntersection(const glm::vec3 & rayOrigin, const glm::vec3 & rayDirection, HitInfo & hitInfo)
	{
		float hitDistance;
		glm::vec3 hitNormal;

		if (!RTMath::RayAABBIntersection(rayOrigin, rayDirection, m_aabb, hitNormal, hitDistance)) return false;

		hitInfo.hitDistance = hitDistance;
		hitInfo.hitNormal = hitNormal;
		return true;
	}

	glm::vec3 BoxLight::Illumination(const glm::vec3 & point, const glm::vec3 & normal, const glm::vec3 & ray)
	{
		glm::vec3 returnIntensity = { 0,0,0 };

		float incidence = std::max(0.0f, glm::dot(glm::normalize(normal), glm::normalize(m_randomDirection)));

		if (incidence > 0.0f)
		{
			float r = glm::distance(m_surfacePoint, point);
			float lightReceived = m_intensity / (r * r);
			returnIntensity = (m_color * lightReceived * incidence);
		}

		return returnIntensity;
	}

	glm::vec3 BoxLight::DirectionToLight(const glm::vec3 & point)
	{
		//srand(static_cast <unsigned> (time(0)));
		float x = (-m_dimensions.x / 2.0f) + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / ((m_dimensions.x / 2.0f) - (-m_dimensions.x / 2.0f))));
		float y = (-m_dimensions.y / 2.0f) + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / ((m_dimensions.y / 2.0f) - (-m_dimensions.y / 2.0f))));
		float z = (-m_dimensions.z / 2.0f) + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / ((m_dimensions.z / 2.0f) - (-m_dimensions.z / 2.0f))));

		glm::vec4 testPoint = m_transform * glm::vec4(x, y, z, 1); // Convert random point in the boxlight to world space

		m_randomDirection = glm::normalize(glm::vec3(testPoint.x, testPoint.y, testPoint.z) - point); // Calculate direction in world space

		glm::vec4 transformedDirection = m_inverseTransform * glm::vec4(m_randomDirection, 0); // Convert direction into model space
		glm::vec4 transformedPoint = m_inverseTransform * glm::vec4(point, 1);

		glm::vec3 tempNormal;
		float distance;

		RTMath::RayAABBIntersection({ transformedPoint.x, transformedPoint.y, transformedPoint.z }, { transformedDirection.x, transformedDirection.y, transformedDirection.z }, m_aabb, tempNormal, distance);

		m_surfacePoint = point + m_randomDirection * distance;

		return m_randomDirection;
	}

	float BoxLight::DistanceToLight(const glm::vec3 & point)
	{		

		return glm::distance(m_surfacePoint, point);
	}
}