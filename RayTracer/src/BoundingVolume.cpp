#include "BoundingVolume.h"
#include "Mesh.h"

namespace RayTracer
{
	BoundingVolume::BoundingVolume(glm::vec3 minDimensions, glm::vec3 maxDimensions, std::vector<std::pair<int, int>>* indices) : m_minDimensions(minDimensions), m_maxDimensions(maxDimensions)
	{
		m_indices = indices;
		m_vertices.push_back(minDimensions);
		m_vertices.push_back(maxDimensions);
		m_vertices.push_back({ minDimensions.x, maxDimensions.y, maxDimensions.z });
		m_vertices.push_back({ maxDimensions.x, minDimensions.y, maxDimensions.z });
		m_vertices.push_back({ maxDimensions.x, maxDimensions.y, minDimensions.z });
		m_vertices.push_back({ minDimensions.x, minDimensions.y, maxDimensions.z });
		m_vertices.push_back({ minDimensions.x, maxDimensions.y, minDimensions.z });
		m_vertices.push_back({ maxDimensions.x, minDimensions.y, minDimensions.z });
	}

	bool BoundingVolume::Intersects(const glm::vec3& rayOrigin, const glm::vec3& rayDirection, float& hitDistance)
	{
		//{
			// Code from http://www.opengl-tutorial.org/miscellaneous/clicking-on-objects/picking-with-custom-ray-obb-function/ modified to work with my program structure
			// Comments were left from the original source for understanding


			// Intersection method from Real-Time Rendering and Essential Mathematics for Games

		//	float tMin = 0.0f;
		//	float tMax = 100000.0f;

		//	glm::vec3 dimensions = m_maxDimensions - m_minDimensions;

		//	glm::vec3 center = dimensions / 2.0f;

		//	glm::vec3 delta = center - rayOrigin;

		//	// Test intersection with the 2 planes perpendicular to the OBB's X axis
		//	{
		//		glm::vec3 xaxis(1, 0, 0);
		//		float e = glm::dot(xaxis, delta);
		//		float f = glm::dot(rayDirection, xaxis);

		//		if (fabs(f) > 0.001f)
		//		{ // Standard case

		//			float t1 = (e + m_minDimensions.x) / f; // Intersection with the "left" plane
		//			float t2 = (e + m_maxDimensions.x) / f; // Intersection with the "right" plane
		//			// t1 and t2 now contain distances betwen ray origin and ray-plane intersections

		//			// We want t1 to represent the nearest intersection, 
		//			// so if it's not the case, invert t1 and t2
		//			if (t1 > t2)
		//			{
		//				float w = t1; t1 = t2; t2 = w; // swap t1 and t2
		//			}

		//			// tMax is the nearest "far" intersection (amongst the X,Y and Z planes pairs)
		//			if (t2 < tMax)
		//				tMax = t2;
		//			// tMin is the farthest "near" intersection (amongst the X,Y and Z planes pairs)
		//			if (t1 > tMin)
		//			{
		//				tMin = t1;
		//			}

		//			// And here's the trick :
		//			// If "far" is closer than "near", then there is NO intersection.
		//			// See the images in the tutorials for the visual explanation.
		//			if (tMax < tMin)
		//				return false;

		//		}
		//		else
		//		{ // Rare case : the ray is almost parallel to the planes, so they don't have any "intersection"
		//			if (-e + m_minDimensions.x > 0.0f || -e + m_maxDimensions.x < 0.0f)
		//				return false;
		//		}
		//	}


		//	// Test intersection with the 2 planes perpendicular to the OBB's Y axis
		//	// Exactly the same thing than above.
		//	{
		//		glm::vec3 yaxis(0, 1, 0);
		//		float e = glm::dot(yaxis, delta);
		//		float f = glm::dot(rayDirection, yaxis);

		//		if (fabs(f) > 0.001f)
		//		{

		//			float t1 = (e + m_minDimensions.y) / f;
		//			float t2 = (e + m_maxDimensions.y) / f;

		//			if (t1 > t2)
		//			{
		//				float w = t1; t1 = t2; t2 = w;
		//			}

		//			if (t2 < tMax)
		//				tMax = t2;
		//			if (t1 > tMin)
		//			{
		//				tMin = t1;
		//			}
		//			if (tMin > tMax)
		//				return false;

		//		}
		//		else
		//		{
		//			if (-e + m_minDimensions.y > 0.0f || -e + m_maxDimensions.y < 0.0f)
		//				return false;
		//		}
		//	}


		//	// Test intersection with the 2 planes perpendicular to the OBB's Z axis
		//	// Exactly the same thing than above.
		//	{
		//		glm::vec3 zaxis(0, 0, 1);
		//		float e = glm::dot(zaxis, delta);
		//		float f = glm::dot(rayDirection, zaxis);

		//		if (fabs(f) > 0.001f)
		//		{

		//			float t1 = (e + m_minDimensions.z) / f;
		//			float t2 = (e + m_maxDimensions.z) / f;

		//			if (t1 > t2)
		//			{
		//				float w = t1; t1 = t2; t2 = w;
		//			}

		//			if (t2 < tMax)
		//				tMax = t2;
		//			if (t1 > tMin)
		//			{
		//				tMin = t1;
		//			}
		//			if (tMin > tMax)
		//				return false;

		//		}
		//		else
		//		{
		//			if (-e + m_minDimensions.z > 0.0f || -e + m_maxDimensions.z < 0.0f)
		//				return false;
		//		}
		//	}

		//	hitDistance = tMin;
		//	indices = m_indices;
		//	return true;
		//}

		float tmin = (m_minDimensions.x - rayOrigin.x) / rayDirection.x;
		float tmax = (m_maxDimensions.x - rayOrigin.x) / rayDirection.x;

		if (tmin > tmax) std::swap(tmin, tmax);

		float tymin = (m_minDimensions.y - rayOrigin.y) / rayDirection.y;
		float tymax = (m_maxDimensions.y - rayOrigin.y) / rayDirection.y;

		if (tymin > tymax) std::swap(tymin, tymax);

		if ((tmin > tymax) || (tymin > tmax))
			return false;

		if (tymin > tmin)
			tmin = tymin;

		if (tymax < tmax)
			tmax = tymax;

		float tzmin = (m_minDimensions.z - rayOrigin.z) / rayDirection.z;
		float tzmax = (m_maxDimensions.z - rayOrigin.z) / rayDirection.z;

		if (tzmin > tzmax) std::swap(tzmin, tzmax);

		if ((tmin > tzmax) || (tzmin > tmax))
			return false;

		if (tzmin > tmin)
			tmin = tzmin;

		if (tzmax < tmax)
			tmax = tzmax;

		hitDistance = tmin;
		return true;

	}

	bool BoundingVolume::IntersectsTriangle(BoundingVolume* volume, TriangleData* triangle)
	{
		// Add AABB axes
		std::vector<glm::vec3> axes;
		axes.push_back({ 1,0,0 });
		axes.push_back({ 0,1,0 });
		axes.push_back({ 0,0,1 });

		// Calculate triangle test axes
		glm::vec3 edgeV0V1 = triangle->v1 - triangle->v0;
		glm::vec3 edgeV0V2 = triangle->v2 - triangle->v0;
		glm::vec3 edgeV1V2 = triangle->v2 - triangle->v1;

		glm::vec3 tAxis0 = glm::cross(edgeV0V1, edgeV0V2);
		tAxis0 = glm::normalize(tAxis0);

		glm::vec3 tAxis1 = glm::cross(edgeV0V1, tAxis0);
		glm::vec3 tAxis2 = glm::cross(edgeV0V2, tAxis0);
		glm::vec3 tAxis3 = glm::cross(edgeV1V2, tAxis0);

		// Add triangle axes
		axes.push_back(tAxis0);
		axes.push_back(tAxis1);
		axes.push_back(tAxis2);
		axes.push_back(tAxis3);

		// Test each axis
		for (int i = 0; i < axes.size(); i++)
		{
			// project AABB onto this axis
			float aabbMin = 1000000.0f;
			float aabbMax = -1000000.0f;

			for (int j = 0; j < volume->m_vertices.size(); j++)
			{
				float value = glm::dot(volume->m_vertices[j], axes[i]);

				if (value < aabbMin) aabbMin = value;
				if (value > aabbMax) aabbMax = value;
			}

			// project triangle onto this axis
			float triangleMin = 1000000.0f;
			float triangleMax = -1000000.0f;

			float value = glm::dot(triangle->v0, axes[i]);
			if (value < triangleMin) triangleMin = value;
			if (value > triangleMax) triangleMax = value;

			value = glm::dot(triangle->v1, axes[i]);
			if (value < triangleMin) triangleMin = value;
			if (value > triangleMax) triangleMax = value;

			value = glm::dot(triangle->v2, axes[i]);
			if (value < triangleMin) triangleMin = value;
			if (value > triangleMax) triangleMax = value;

			// No overlap, triangle is not in this volume
			if ((triangleMax < aabbMin) || (triangleMin > aabbMax)) return false;
		}

		// There was no separating axis -> the triangle is in this volume
		return true;
	}
}