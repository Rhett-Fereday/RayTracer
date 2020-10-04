#pragma once

#include "../Core.h"
#include "AreaLight.h"
#include "../RTMath.h"

namespace RayTracer
{
	class RT_API BoxLight : public AreaLight
	{
	public:
		BoxLight(glm::mat4 transform, glm::vec3 dimensions, ConstMaterial* material, glm::vec3 color, float intensity);
		bool TestIntersection(const glm::vec3& rayOrigin, const glm::vec3& rayDirection, HitInfo& hitInfo);
		glm::vec3 Illumination(const glm::vec3& point, const glm::vec3& normal, const glm::vec3& ray);
		glm::vec3 DirectionToLight(const glm::vec3& point);
		float DistanceToLight(const glm::vec3 & point);

	private:
		RTMath::AABB m_aabb;
		glm::vec3 m_dimensions, m_randomDirection, m_surfacePoint;
	};
}