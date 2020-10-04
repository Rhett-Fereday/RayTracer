#include "Mesh.h"
#define TINYOBJLOADER_IMPLEMENTATION
#include "tiny_obj_loader.h"
#include "../BVH.h"
#include <string>

namespace RayTracer
{
	Mesh::Mesh(const char* file, int bvhDepth)
	{
		m_shapes = new std::vector<tinyobj::shape_t>();
		m_materials = new std::vector<tinyobj::material_t>();
		m_attrib = new tinyobj::attrib_t();

		std::string warning, error;
		bool loaded = tinyobj::LoadObj(m_attrib, m_shapes, m_materials, &warning, &error, file);

		m_bvh = new BVH(this, bvhDepth);
	}

	bool Mesh::Intersects(const glm::vec3 & rayOrigin, const glm::vec3 & rayDirection, HitInfo & hitInfo)
	{
		// check BVH for intersection
		if (!m_bvh->Intersects(rayOrigin, rayDirection, hitInfo)) return false;

		return true;
	}

	RTMath::Triangle Mesh::GetTriangleData(int shapeIndex, int meshIndex)
	{
		RTMath::Triangle returnData;

		int v0Index = (*m_shapes)[shapeIndex].mesh.indices[meshIndex].vertex_index * 3;
		int v1Index = (*m_shapes)[shapeIndex].mesh.indices[meshIndex + 1].vertex_index * 3;
		int v2Index = (*m_shapes)[shapeIndex].mesh.indices[meshIndex + 2].vertex_index * 3;

		int n0Index = (*m_shapes)[shapeIndex].mesh.indices[meshIndex].normal_index * 3;
		int n1Index = (*m_shapes)[shapeIndex].mesh.indices[meshIndex + 1].normal_index * 3;
		int n2Index = (*m_shapes)[shapeIndex].mesh.indices[meshIndex + 2].normal_index * 3;

		returnData.v0 = { m_attrib->vertices[v0Index], m_attrib->vertices[v0Index + 1], m_attrib->vertices[v0Index + 2] };
		returnData.v1 = { m_attrib->vertices[v1Index], m_attrib->vertices[v1Index + 1], m_attrib->vertices[v1Index + 2] };
		returnData.v2 = { m_attrib->vertices[v2Index], m_attrib->vertices[v2Index + 1], m_attrib->vertices[v2Index + 2] };

		returnData.n0 = { m_attrib->normals[n0Index], m_attrib->normals[n0Index + 1], m_attrib->normals[n0Index + 2] };
		returnData.n1 = { m_attrib->normals[n1Index], m_attrib->normals[n1Index + 1], m_attrib->normals[n1Index + 2] };
		returnData.n2 = { m_attrib->normals[n2Index], m_attrib->normals[n2Index + 1], m_attrib->normals[n2Index + 2] };

		return returnData;
	}
}