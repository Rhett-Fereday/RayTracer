#pragma once

#include "../Core.h"
#include "AreaLight.h"
#include "../RTMath.h"

namespace RayTracer
{
	class RT_API RectLight : public AreaLight
	{
	public:
		RectLight(glm::mat4 transform, glm::vec3 dimensions, ConstMaterial* material, glm::vec3 color, float intensity);
		glm::vec3 SampleRadiance(const glm::vec3& point, const glm::vec3& normal, glm::vec3 &sampleDirection, float &pdf, float &sampleDistance);

	private:
		RTMath::AABB m_aabb;
		glm::vec3 m_dimensions;
	};
}