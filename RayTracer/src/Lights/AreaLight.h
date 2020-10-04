#pragma once

#include "Light.h"
#include "../Objects/Object.h"
#include "../ConstMaterial.h"
#include "glm/glm.hpp"

namespace RayTracer
{
	class AreaLight : public Light, public Object
	{
	public:
		AreaLight(glm::mat4 transform, ConstMaterial* material, glm::vec3 color, float intensity);
		virtual bool TestIntersection(const glm::vec3& rayOrigin, const glm::vec3& rayDirection, HitInfo& hitInfo) = 0;
		virtual glm::vec3 Illumination(const glm::vec3& point, const glm::vec3& normal, const glm::vec3& ray) = 0;
		virtual glm::vec3 DirectionToLight(const glm::vec3& point) = 0;
		virtual float DistanceToLight(const glm::vec3 & point) = 0;
	};
}