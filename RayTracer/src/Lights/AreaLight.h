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
		AreaLight(glm::mat4 transform, ConstMaterial* material);
		virtual glm::vec3 SampleRadiance(const glm::vec3& point, const glm::vec3& normal, glm::vec3 &sampleDirection, float &pdf, float &sampleDistance) = 0;
	
	protected:
		bool TestIntersection(const glm::vec3& rayOrigin, const glm::vec3& rayDirection, HitInfo& hitInfo) = 0;
	};
}