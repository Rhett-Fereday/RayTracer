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
		virtual glm::vec3 SampleRadiance(const glm::vec3& point, const glm::vec3& normal, glm::vec3 &sampleDirection, float &pdf, float &sampleDistance) = 0;
	};
}