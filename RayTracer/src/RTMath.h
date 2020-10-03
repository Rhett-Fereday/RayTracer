#pragma once

#include "glm/glm.hpp"

namespace RayTracer
{
	namespace RTMath
	{
		struct AABB
		{
			glm::vec3 minDimensions;
			glm::vec3 maxDimensions;
		};

		struct Triangle
		{
			glm::vec3 v0, n0;
			glm::vec3 v1, n1;
			glm::vec3 v2, n2;
		};

		bool RaySphereIntersection(const glm::vec3 &rayOrigin, const glm::vec3 &rayDirection, const float &radius, glm::vec3 &hitNormal, float &hitDistance);
		bool RayAABBIntersection(const glm::vec3 &rayOrigin, const glm::vec3 &rayDirection, const AABB& aabb, glm::vec3 &hitNormal, float &hitDistance);
		bool RayTriangleIntersection(const glm::vec3 &rayOrigin, const glm::vec3 &rayDirection, const Triangle &triangle, glm::vec3 &hitNormal, float &hitDistance);
		bool TriangleAABBIntersection(const AABB& aabb, const Triangle &triangle);
	}
}