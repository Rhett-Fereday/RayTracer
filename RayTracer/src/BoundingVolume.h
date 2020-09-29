#pragma once

#include <vector>
#include "glm/glm.hpp"

namespace RayTracer
{
	struct TriangleData;

	class BoundingVolume
	{
	public:
		std::vector<std::pair<int, int>>* m_indices;

		BoundingVolume(glm::vec3 minDimensions, glm::vec3 maxDimensions, std::vector<std::pair<int, int>>* indices = nullptr);
		bool Intersects(const glm::vec3& rayOrigin, const glm::vec3& rayDirection, float &hitDistance);

		static bool IntersectsTriangle(BoundingVolume* volume, TriangleData* triangle);

	private:
		glm::vec3 m_minDimensions, m_maxDimensions;
		std::vector<glm::vec3> m_vertices;
	};
}