#pragma once

#include "glm/glm.hpp"

namespace RayTracer
{
	// Base class to allow storing all light types in a single vector
	class Light
	{
	public:
		Light(glm::vec3 color, float intensity);
		virtual glm::vec3 SampleRadiance(const glm::vec3& point, const glm::vec3& normal, glm::vec3 &sampleDirection, float &pdf, float &sampleDistance) = 0;

	protected:
		glm::vec3 m_color;
		float m_intensity;
	};
}