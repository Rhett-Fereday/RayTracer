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

		bool RayPlaneIntersection(const glm::vec3 &rayOrigin, const glm::vec3 &rayDirection, const float width, const float length, float &hitDistance);
		bool RaySphereIntersection(const glm::vec3 &rayOrigin, const glm::vec3 &rayDirection, const float &radius, glm::vec3 &hitNormal, float &hitDistance, bool &insideObject);
		bool RayAABBIntersection(const glm::vec3 &rayOrigin, const glm::vec3 &rayDirection, const AABB& aabb, glm::vec3 &hitNormal, float &hitDistance, bool &insideObject);
		bool RayTriangleIntersection(const glm::vec3 &rayOrigin, const glm::vec3 &rayDirection, const Triangle &triangle, glm::vec3 &hitNormal, float &hitDistance, bool &insideObject);
		bool TriangleAABBIntersection(const AABB& aabb, const Triangle &triangle);
		float Fresnel(const glm::vec3 &incomingDirection, const glm::vec3 &normal, const float &refractiveIndex);
		float DistributionGGX(const glm::vec3 &normal, const glm::vec3 &halfway, float roughness);
		float GeometrySchlickGGX(float NdotV, float roughness);
		float GeometrySmith(const glm::vec3 N, const glm::vec3 V, const glm::vec3 L, float k);
		glm::vec3 FresnelSchlick(float cosTheta, const glm::vec3 F0);
		float Gaussian(float distance, float sigma);
	}
}