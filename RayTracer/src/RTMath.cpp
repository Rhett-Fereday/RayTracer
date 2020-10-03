#include "RTMath.h"
#include <vector>

namespace RayTracer
{
	namespace RTMath
	{
		using namespace glm;

		
		//Determine whether or not a ray intersects a sphere of a given radius.
		//This method assumes the rayOrigin and rayDirection are in model space.
		bool RaySphereIntersection(const glm::vec3 & rayOrigin, const glm::vec3 & rayDirection, const float & radius, glm::vec3 & hitNormal, float & hitDistance)
		{
			float a = 1.0f;//dot(rayDirection, rayDirection); Since it's normalized this is always 1
			float b = dot(2.0f * rayDirection, rayOrigin);
			float c = dot(rayOrigin, rayOrigin) - (radius * radius);

			float radicand = b * b - 4 * a * c;

			if (radicand < 0) return false;

			float u1, u2;
			glm::vec3 intersect1, intersect2;
			float root = sqrt(radicand);
			u1 = (-b + root) / (2.0f * a);
			u2 = (-b - root) / (2.0f * a);

			if (u2 < u1) u1 = u2;

			// Account for floating point inaccuracy
			if (u1 < 0.0001) return false;

			vec3 hitPosition = rayOrigin + rayDirection * u1;
			hitNormal = normalize(hitPosition);
			hitDistance = u1;

			return true;
		}

		bool RayAABBIntersection(const glm::vec3 & rayOrigin, const glm::vec3 & rayDirection, const AABB& aabb, glm::vec3 &hitNormal, float & hitDistance)
		{
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

			if (tmin < 0.0001f) return false;

			hitDistance = tmin;
			return true;
		}

		//Determine whether or not a ray intersects an axis aligned bounding box of given dimensions.
		//This method assumes the rayOrigin and rayDirection are in model space.
		//bool RayAABBIntersection(const glm::vec3 & rayOrigin, const glm::vec3 & rayDirection, const AABB& aabb, glm::vec3 & hitNormal, float & hitDistance)
		//{
		//	{
		//		// Code from http://www.opengl-tutorial.org/miscellaneous/clicking-on-objects/picking-with-custom-ray-obb-function/ modified to work with my program structure
		//		// Comments were left from the original source for understanding


		//		// Intersection method from Real-Time Rendering and Essential Mathematics for Games

		//		float tMin = 0.0f;
		//		float tMax = 100000.0f;

		//		vec3 delta = -rayOrigin;

		//		// Test intersection with the 2 planes perpendicular to the OBB's X axis
		//		{
		//			vec3 xaxis(1, 0, 0);
		//			float e = dot(xaxis, delta);
		//			float f = dot(rayDirection, xaxis);
		//			vec3 tempNormal = xaxis;

		//			if (fabs(f) > 0.001f)
		//			{ // Standard case

		//				float t1 = (e + aabb.minDimensions.x) / f; // Intersection with the "left" plane
		//				float t2 = (e + aabb.maxDimensions.x) / f; // Intersection with the "right" plane
		//				// t1 and t2 now contain distances betwen ray origin and ray-plane intersections

		//				// We want t1 to represent the nearest intersection, 
		//				// so if it's not the case, invert t1 and t2
		//				if (t1 > t2)
		//				{
		//					float w = t1; t1 = t2; t2 = w; // swap t1 and t2
		//					tempNormal = -tempNormal;
		//				}

		//				// tMax is the nearest "far" intersection (amongst the X,Y and Z planes pairs)
		//				if (t2 < tMax)
		//					tMax = t2;
		//				// tMin is the farthest "near" intersection (amongst the X,Y and Z planes pairs)
		//				if (t1 > tMin)
		//				{
		//					tMin = t1;
		//					hitNormal = -tempNormal;
		//				}

		//				// And here's the trick :
		//				// If "far" is closer than "near", then there is NO intersection.
		//				// See the images in the tutorials for the visual explanation.
		//				if (tMax < tMin)
		//					return false;

		//			}
		//			else
		//			{ // Rare case : the ray is almost parallel to the planes, so they don't have any "intersection"
		//				if (-e + aabb.minDimensions.x > 0.0f || -e + aabb.maxDimensions.x < 0.0f)
		//					return false;
		//			}
		//		}


		//		// Test intersection with the 2 planes perpendicular to the OBB's Y axis
		//		// Exactly the same thing than above.
		//		{
		//			glm::vec3 yaxis(0, 1, 0);
		//			float e = dot(yaxis, delta);
		//			float f = dot(rayDirection, yaxis);
		//			vec3 tempNormal = yaxis;

		//			if (fabs(f) > 0.001f)
		//			{

		//				float t1 = (e + aabb.minDimensions.y) / f;
		//				float t2 = (e + aabb.maxDimensions.y) / f;

		//				if (t1 > t2)
		//				{
		//					float w = t1; t1 = t2; t2 = w;
		//					tempNormal = -tempNormal;
		//				}

		//				if (t2 < tMax)
		//					tMax = t2;
		//				if (t1 > tMin)
		//				{
		//					tMin = t1;
		//					hitNormal = -tempNormal;
		//				}
		//				if (tMin > tMax)
		//					return false;

		//			}
		//			else
		//			{
		//				if (-e + aabb.minDimensions.y > 0.0f || -e + aabb.maxDimensions.y < 0.0f)
		//					return false;
		//			}
		//		}


		//		// Test intersection with the 2 planes perpendicular to the OBB's Z axis
		//		// Exactly the same thing than above.
		//		{
		//			glm::vec3 zaxis(0, 0, 1);
		//			float e = dot(zaxis, delta);
		//			float f = dot(rayDirection, zaxis);
		//			glm::vec3 tempNormal = zaxis;

		//			if (fabs(f) > 0.001f)
		//			{

		//				float t1 = (e + aabb.minDimensions.z) / f;
		//				float t2 = (e + aabb.maxDimensions.z) / f;

		//				if (t1 > t2)
		//				{
		//					float w = t1; t1 = t2; t2 = w;
		//					tempNormal = -tempNormal;
		//				}

		//				if (t2 < tMax)
		//					tMax = t2;
		//				if (t1 > tMin)
		//				{
		//					tMin = t1;
		//					hitNormal = -tempNormal;
		//				}
		//				if (tMin > tMax)
		//					return false;

		//			}
		//			else
		//			{
		//				if (-e + aabb.minDimensions.z > 0.0f || -e + aabb.maxDimensions.z < 0.0f)
		//					return false;
		//			}
		//		}

		//		hitDistance = tMin;
		//		return true;
		//	}
		//}

		//Determine whether or not a ray intersects a triangle defined by the triangle data.
		//This method assumes the rayOrigin and rayDirection are in model space.
		bool RayTriangleIntersection(const glm::vec3 & rayOrigin, const glm::vec3 & rayDirection, const Triangle & triangle, glm::vec3 & hitNormal, float & hitDistance)
		{
			float u, v;

			glm::vec3 v0v1 = triangle.v1 - triangle.v0;
			glm::vec3 v0v2 = triangle.v2 - triangle.v0;
			glm::vec3 pvec = cross(rayDirection, v0v2);
			float det = dot(v0v1, pvec);

			if (fabs(det) < 0.0001) return false;

			float invDet = 1.0f / det;

			glm::vec3 tvec = rayOrigin - triangle.v0;
			u = glm::dot(tvec, pvec) * invDet;
			if (u < 0 || u > 1) return false;

			vec3 qvec = cross(tvec, v0v1);
			v = dot(rayDirection, qvec) * invDet;

			if (v < 0 || u + v > 1) return false;

			hitDistance = dot(v0v2, qvec) * invDet;

			if (hitDistance < 0.0001) return false;

			hitNormal = normalize((1.0f - u - v) * triangle.n0 + u * triangle.n1 + v * triangle.n2);

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
	}
}