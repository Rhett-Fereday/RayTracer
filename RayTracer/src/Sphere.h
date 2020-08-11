#pragma once

#include "glm/glm.hpp"

namespace RayTracer
{
	class Sphere
	{
	public:
		Sphere(float radius, glm::vec3 albedo);

	private:
		float m_radius;
		glm::vec3 m_albedo;
	};
}