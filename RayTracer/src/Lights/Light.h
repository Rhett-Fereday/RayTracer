#pragma once

#include "glm/glm.hpp"

namespace RayTracer
{
	// Base class to allow storing all light types in a single vector
	class Light
	{
	public:
		Light(glm::vec3 color, float intensity);
		virtual glm::vec3 Illumination(const glm::vec3& point, const glm::vec3& normal, const glm::vec3& ray) = 0;
		virtual glm::vec3 DirectionToLight(const glm::vec3& point) = 0;
		virtual float DistanceToLight(const glm::vec3 & point) = 0;

	protected:
		glm::vec3 m_color;
		float m_intensity;
	};
}