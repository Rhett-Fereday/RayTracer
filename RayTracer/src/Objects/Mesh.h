#pragma once

#include "../Core.h"
#include "Object.h"
#include "Box.h"
#include "../RTMath.h"
#include <vector>

namespace tinyobj
{
	struct attrib_t;
	struct shape_t;
	struct material_t;
}

namespace RayTracer
{
	class BVH;

	class RT_API Mesh
	{
		friend class BVH;

	public:
		Mesh(const char* file, int bvhDepth);
		bool Intersects(const glm::vec3& rayOrigin, const glm::vec3& rayDirection, HitInfo& hitInfo);

	private:
		BVH* m_bvh;
		tinyobj::attrib_t* m_attrib;
		std::vector<tinyobj::shape_t>* m_shapes;
		std::vector<tinyobj::material_t>* m_materials;

		RTMath::Triangle GetTriangleData(int shapeIndex, int meshIndex);
	};
}