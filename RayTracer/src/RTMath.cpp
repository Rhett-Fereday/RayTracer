#include "RTMath.h"
#include <vector>
#include <algorithm>

namespace RayTracer
{
	namespace RTMath
	{
		using namespace glm;

		//Determine whether or not a ray intersects a plane
		//This method assumes the rayOrigin and rayDirection are in model space. 
		//It also assumes the "face" of the plane faces the positive z direction in model space
		bool RayPlaneIntersection(const glm::vec3 &rayOrigin, const glm::vec3 &rayDirection, const float width, const float length, float &hitDistance)
		{
			glm::vec3 planeNormal = { 0,0,1 };

			// assuming vectors are all normalized
			float denom = dot(planeNormal, -rayDirection);

			if (denom > 1e-6) 
			{
				//glm::vec3 p0l0 = -rayOrigin;
				hitDistance = dot(rayOrigin, planeNormal) / denom;

				glm::vec3 hitPoint = rayOrigin + hitDistance * rayDirection;
				float distanceFromCenter = distance(hitPoint, { 0,0,0 });

				if (hitDistance >= 0)
				{
					float x = fabs(hitPoint.x);
					float y = fabs(hitPoint.y);

					if ((x > width / 2.0f) || (y > length / 2.0f)) return false;

					return true;
				}
			}

			return false;
		}
		
		//Determine whether or not a ray intersects a sphere of a given radius.
		//This method assumes the rayOrigin and rayDirection are in model space.
		bool RaySphereIntersection(const glm::vec3 & rayOrigin, const glm::vec3 & rayDirection, const float & radius, glm::vec3 & hitNormal, float & hitDistance, bool &insideObject)
		{
			insideObject = false;
			float a = dot(rayDirection, rayDirection);
			float b = dot(2.0f * rayDirection, rayOrigin);
			float c = dot(rayOrigin, rayOrigin) - (radius * radius);

			float radicand = b * b - 4 * a * c;

			if (radicand < 0) return false;

			float u1, u2;
			glm::vec3 intersect1, intersect2;
			float root = sqrt(radicand);
			u1 = (-b + root) / (2.0f * a);
			u2 = (-b - root) / (2.0f * a);

			if (u2 < u1) std::swap(u1, u2);

			if (u1 < 0.0001 && u2 < 0.0001) return false;

			vec3 hitPosition = rayOrigin + rayDirection * u1;
			hitDistance = u1;
			hitNormal = hitPosition;

			if (u1 < 0.0001)
			{
				hitPosition = rayOrigin + rayDirection * u2;
				hitDistance = u2;
				hitNormal = -hitPosition;
				insideObject = true;
			}

			hitNormal = normalize(hitNormal);
			return true;
		}

		bool RayAABBIntersection(const glm::vec3 & rayOrigin, const glm::vec3 & rayDirection, const AABB& aabb, glm::vec3 &hitNormal, float & hitDistance, bool &insideObject)
		{
			insideObject = false;
			float tmin = (aabb.minDimensions.x - rayOrigin.x) / rayDirection.x;
			float tmax = (aabb.maxDimensions.x - rayOrigin.x) / rayDirection.x;

			hitNormal = { -1, 0, 0 };

			if (tmin > tmax)
			{
				std::swap(tmin, tmax);
				hitNormal = { 1,0,0 };
			}

			float tymin = (aabb.minDimensions.y - rayOrigin.y) / rayDirection.y;
			float tymax = (aabb.maxDimensions.y - rayOrigin.y) / rayDirection.y;

			glm::vec3 tempNormal = { 0, -1, 0 };

			if (tymin > tymax)
			{
				std::swap(tymin, tymax);
				tempNormal = { 0, 1, 0 };
			}

			if ((tmin > tymax) || (tymin > tmax))
				return false;

			if (tymin > tmin)
			{
				tmin = tymin;
				hitNormal = tempNormal;
			}

			if (tymax < tmax)
				tmax = tymax;

			float tzmin = (aabb.minDimensions.z - rayOrigin.z) / rayDirection.z;
			float tzmax = (aabb.maxDimensions.z - rayOrigin.z) / rayDirection.z;

			tempNormal = { 0, 0, -1 };

			if (tzmin > tzmax)
			{
				std::swap(tzmin, tzmax);
				tempNormal = { 0, 0, 1 };
			}

			if ((tmin > tzmax) || (tzmin > tmax))
				return false;

			if (tzmin > tmin)
			{
				tmin = tzmin;
				hitNormal = tempNormal;
			}

			if (tzmax < tmax)
				tmax = tzmax;

			if ((tmin < 0.0001f) && (tmax < 0.0001f)) return false;

			hitDistance = tmin;

			if (tmin < 0.0001f)
			{
				hitDistance = tmax;
				insideObject = true;
			}

			return true;
		}

		//Determine whether or not a ray intersects a triangle defined by the triangle data.
		//This method assumes the rayOrigin and rayDirection are in model space.
		bool RayTriangleIntersection(const glm::vec3 & rayOrigin, const glm::vec3 & rayDirection, const Triangle & triangle, glm::vec3 & hitNormal, float & hitDistance, bool &insideObject)
		{
			insideObject = false;
			float u, v;

			glm::vec3 v0v1 = triangle.v1 - triangle.v0;
			glm::vec3 v0v2 = triangle.v2 - triangle.v0;
			glm::vec3 pvec = cross(rayDirection, v0v2);
			float det = dot(v0v1, pvec);

			if (fabs(det) < 0.0001) return false;
			
			if (det < 0.0001) insideObject = true;

			float invDet = 1.0f / det;

			glm::vec3 tvec = rayOrigin - triangle.v0;
			u = glm::dot(tvec, pvec) * invDet;
			if (u < 0 || u > 1) return false;

			vec3 qvec = cross(tvec, v0v1);
			v = dot(rayDirection, qvec) * invDet;

			if (v < 0 || u + v > 1) return false;

			hitDistance = dot(v0v2, qvec) * invDet;

			if (hitDistance < 0.0001) return false;

			hitNormal = (1.0f - u - v) * triangle.n0 + u * triangle.n1 + v * triangle.n2;

			if (insideObject) hitNormal = -hitNormal;

			hitNormal = glm::normalize(hitNormal);

			return true;
		}

		//Determine whether or not there is overlap between the given triangle and given AABB using the Separating Axis Theorem.
		//This method assumes they are in the same coordinate frame
		bool TriangleAABBIntersection(const AABB& aabb, const Triangle & triangle)
		{
			vec3 aabbVertices[8];
			aabbVertices[0] = aabb.minDimensions;
			aabbVertices[1] = aabb.maxDimensions;
			aabbVertices[2] = { aabb.minDimensions.x, aabb.maxDimensions.y, aabb.maxDimensions.z };
			aabbVertices[3] = { aabb.maxDimensions.x, aabb.minDimensions.y, aabb.maxDimensions.z };
			aabbVertices[4] = { aabb.maxDimensions.x, aabb.maxDimensions.y, aabb.minDimensions.z };
			aabbVertices[5] = { aabb.minDimensions.x, aabb.minDimensions.y, aabb.maxDimensions.z };
			aabbVertices[6] = { aabb.minDimensions.x, aabb.maxDimensions.y, aabb.minDimensions.z };
			aabbVertices[7] = { aabb.maxDimensions.x, aabb.minDimensions.y, aabb.minDimensions.z };

			// Add AABB axes
			vec3 axes[7];
			axes[0] = { 1,0,0 };
			axes[1] = { 0,1,0 };
			axes[2] = { 0,0,1 };

			// Calculate triangle test axes
			vec3 edgeV0V1 = triangle.v1 - triangle.v0;
			vec3 edgeV0V2 = triangle.v2 - triangle.v0;
			vec3 edgeV1V2 = triangle.v2 - triangle.v1;

			vec3 tAxis0 = cross(edgeV0V1, edgeV0V2);
			tAxis0 = normalize(tAxis0);

			vec3 tAxis1 = normalize(cross(edgeV0V1, tAxis0));
			vec3 tAxis2 = normalize(cross(edgeV0V2, tAxis0));
			vec3 tAxis3 = normalize(cross(edgeV1V2, tAxis0));

			// Add triangle axes
			axes[3] = tAxis0;
			axes[4] = tAxis1;
			axes[5] = tAxis2;
			axes[6] = tAxis3;

			// Test each axis
			for (int i = 0; i < 7; i++)
			{
				// project AABB onto this axis
				float aabbMin = 1000000.0f;
				float aabbMax = -1000000.0f;

				for (int j = 0; j < 8; j++)
				{
					float value = dot(aabbVertices[j], axes[i]);

					if (value < aabbMin) aabbMin = value;
					if (value > aabbMax) aabbMax = value;
				}

				// project triangle onto this axis
				float triangleMin = 1000000.0f;
				float triangleMax = -1000000.0f;

				float value = dot(triangle.v0, axes[i]);
				if (value < triangleMin) triangleMin = value;
				if (value > triangleMax) triangleMax = value;

				value = dot(triangle.v1, axes[i]);
				if (value < triangleMin) triangleMin = value;
				if (value > triangleMax) triangleMax = value;

				value = dot(triangle.v2, axes[i]);
				if (value < triangleMin) triangleMin = value;
				if (value > triangleMax) triangleMax = value;

				// No overlap, triangle is not in this volume
				if ((triangleMax < aabbMin) || (triangleMin > aabbMax)) return false;
			}

			// There was no separating axis -> the triangle is in this volume
			return true;
		}

		float Fresnel(const glm::vec3 & incomingDirection, const glm::vec3 & normal, const float & refractiveIndex)
		{
			float reflectivePortion;
			float cosi = dot(incomingDirection, normal);
			float etai = 1.0f, etat = refractiveIndex;

			if (cosi > 0) std::swap(etai, etat);

			// Compute sini using Snell's law
			float sint = etai / etat * sqrtf(std::max(0.0f, 1.0f - cosi * cosi));

			// Total internal reflection
			if (sint >= 1) 
			{
				reflectivePortion = 1.0f;
			}
			else 
			{
				float cost = sqrtf(std::max(0.0f, 1.0f - sint * sint));
				cosi = fabsf(cosi);
				float Rs = ((etat * cosi) - (etai * cost)) / ((etat * cosi) + (etai * cost));
				float Rp = ((etai * cosi) - (etat * cost)) / ((etai * cosi) + (etat * cost));
				reflectivePortion = (Rs * Rs + Rp * Rp) / 2.0f;
			}

			return reflectivePortion;
		}

		float DistributionGGX(const vec3 &N, const vec3 &H, float roughness)
		{
			float a = roughness * roughness;
			float a2 = a * a;
			float NdotH = max(dot(N, H), 0.0f);
			float NdotH2 = NdotH * NdotH;

			float num = a2;
			float denom = (NdotH2 * (a2 - 1.0) + 1.0);
			denom = 3.14f * denom * denom;

			return num / denom;
		}

		float GeometrySchlickGGX(float NdotV, float roughness)
		{
			float r = (roughness + 1.0);
			float k = (r*r) / 8.0;

			float num = NdotV;
			float denom = NdotV * (1.0 - k) + k;

			return num / denom;
		}

		float GeometrySmith(vec3 N, vec3 V, vec3 L, float roughness)
		{
			float NdotV = max(dot(N, V), 0.0f);
			float NdotL = max(dot(N, L), 0.0f);
			float ggx2 = GeometrySchlickGGX(NdotV, roughness);
			float ggx1 = GeometrySchlickGGX(NdotL, roughness);

			return ggx1 * ggx2;
		}

		glm::vec3 FresnelSchlick(float cosTheta, const glm::vec3 F0)
		{
			return F0 + (1.0f - F0) * pow(1.0f - cosTheta, 5.0f);
		}
	}
}