#pragma once

#include "Core.h"
#include "Light.h"
#include "glm/glm.hpp"

namespace RayTracer
{
	class RT_API PointLight : public Light
	{
	public:
		PointLight(glm::vec3 color, float intensity, glm::vec3 position);
		glm::vec3 Illumination(const glm::vec3& point, const glm::vec3& normal, const glm::vec3& ray);
		glm::vec3 DirectionToLight(const glm::vec3& point);
		float DistanceToLight(const glm::vec3 & point);

	private:
		glm::vec3 m_position;
	};
}