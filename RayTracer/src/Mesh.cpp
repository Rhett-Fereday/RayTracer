#include "Mesh.h"
#define TINYOBJLOADER_IMPLEMENTATION
#include "tiny_obj_loader.h"
#include "BoundingVolume.h"
#include "BVH.h"
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

		/*glm::vec3 minValues = {100000, 100000, 100000 };
		glm::vec3 maxValues = { -100000, -100000, -100000 };

		for (int i = 0; i < m_attrib->vertices.size(); i += 3)
		{
			glm::vec3 vertex = {m_attrib->vertices[i], m_attrib->vertices[i + 1], m_attrib->vertices[i + 2] };

			if (vertex.x < minValues.x) minValues.x = vertex.x;
			if (vertex.x > maxValues.x) maxValues.x = vertex.x;

			if (vertex.y < minValues.y) minValues.y = vertex.y;
			if (vertex.y > maxValues.y) maxValues.y = vertex.y;

			if (vertex.z < minValues.z) minValues.z = vertex.z;
			if (vertex.z > maxValues.z) maxValues.z = vertex.z;
		}

		std::vector<int>* indices = new std::vector<int>();

		for (int i = 0; i < (*m_shapes)[0].mesh.indices.size(); i += 3)
		{
			indices->push_back(i);
		}

		BoundingVolume* bv = new BoundingVolume({ 0,0,0 }, minValues, maxValues, indices);
		m_bvh->AddVolume(-1, bv);*/
	}

	bool Mesh::Intersects(const glm::vec3 & rayOrigin, const glm::vec3 & rayDirection, HitInfo & hitInfo)
	{
		// check BVH for intersection
		if (!m_bvh->Intersects(rayOrigin, rayDirection, hitInfo)) return false;

		return true;

		/*HitInfo boxCheck;
		if (!m_boundingBox->Intersects(rayOrigin, rayDirection, boxCheck)) return false;*/

		//float closestDistance = 10000.0f;
		//bool returnValue = false;

		//// Loop over every shape present in the model
		//for (int i = 0; i < m_shapes->size(); i++)
		//{
		//	// Loop over all of the triangles in the mesh of this shape
		//	for (int j = 0; j < (*m_shapes)[i].mesh.indices.size(); j += 3)
		//	{
		//		HitInfo tempInfo;
		//		
		//		TriangleData triangle = GetTriangleData(i, j);

		//		// Test the triangle and update closest hit information
		//		if (RayTriangleIntersect(rayOrigin, rayDirection, triangle, tempInfo))
		//		{
		//			if (tempInfo.hitDistance < closestDistance)
		//			{
		//				closestDistance = tempInfo.hitDistance;
		//				hitInfo = tempInfo;
		//				returnValue = true;
		//			}
		//		}
		//	}
		//}

		//return returnValue;
	}

	// Ray triangle intersection using the Moller-Trumbore algorithm (scratchapixel.com)
	bool Mesh::RayTriangleIntersect(
		const glm::vec3 &rayOrigin, const glm::vec3 &rayDirection,
		const TriangleData &triangle,
		HitInfo &hitInfo)
	{
		float u, v;

		glm::vec3 v0v1 = triangle.v1 - triangle.v0;
		glm::vec3 v0v2 = triangle.v2 - triangle.v0;
		glm::vec3 pvec = glm::cross(rayDirection, v0v2);
		float det = glm::dot(v0v1, pvec);

		if (fabs(det) < 0.0001) return false;

		float invDet = 1.0f / det;

		glm::vec3 tvec = rayOrigin - triangle.v0;
		u = glm::dot(tvec, pvec) * invDet;
		if (u < 0 || u > 1) return false;

		glm::vec3 qvec = glm::cross(tvec, v0v1);
		v = glm::dot(rayDirection, qvec) * invDet;

		if (v < 0 || u + v > 1) return false;

		hitInfo.hitDistance = glm::dot(v0v2, qvec) * invDet;

		if (hitInfo.hitDistance < 0.0001) return false;

		//hitInfo.hitPosition = rayOrigin + rayDirection * hitInfo.hitDistance;
		//hitInfo.hitMaterial = m_material;

		hitInfo.hitNormal = glm::normalize((1.0f - u - v) * triangle.n0 + u * triangle.n1 + v * triangle.n2);
		//glm::vec4 tempNormal = m_transform * glm::vec4(interpolatedNormal, 0);
		//hitInfo.hitNormal = { tempNormal.x, tempNormal.y, tempNormal.z };

		return true;
	}

	TriangleData Mesh::GetTriangleData(int shapeIndex, int meshIndex)
	{
		TriangleData returnData;

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