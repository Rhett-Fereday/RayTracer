#pragma once

#include "Core.h"
#include "Object.h"
#include "Box.h"
#include <vector>

namespace tinyobj
{
	struct attrib_t;
	struct shape_t;
	struct material_t;
}

namespace RayTracer
{
	struct TriangleData
	{
		glm::vec3 v0, n0;
		glm::vec3 v1, n1;
		glm::vec3 v2, n2;
	};

	class RT_API Mesh
	{
	public:
		Mesh(const char* file);
		bool Intersects(const glm::vec3& rayOrigin, const glm::vec3& rayDirection, HitInfo& hitInfo);

	private:
		tinyobj::attrib_t* m_attrib;
		std::vector<tinyobj::shape_t>* m_shapes;
		std::vector<tinyobj::material_t>* m_materials;
		
		bool RayTriangleIntersect(
			const glm::vec3 &rayOrigin, const glm::vec3 &rayDirection,
			const TriangleData &triangle,
			HitInfo &hitInfo);

		TriangleData GetTriangleData(int shapeIndex, int meshIndex);
	};
}