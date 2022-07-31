#include "Box.h"
#include "../../RTMath.h"
#include "../../Interactions/SurfaceInteraction.h"
//#include "Box.h"
//#include "../RTMath.h"
//
//namespace RayTracer
//{
//	Box::Box(glm::mat4 transform, ConstMaterial* material, glm::vec3 dimensions) : Object(transform, material)
//	{
//		float xDim = dimensions.x / 2.0f;
//		float yDim = dimensions.y / 2.0f;
//		float zDim = dimensions.z / 2.0f;
//
//		m_aabb.minDimensions = { -xDim, -yDim, -zDim };
//		m_aabb.maxDimensions = { xDim, yDim, zDim };
//	}
//
//	Box::Box(glm::mat4 transform, ConstMaterial * material, glm::vec3 minDimensions, glm::vec3 maxDimensions) : Object(transform, material)
//	{
//		m_aabb.minDimensions = minDimensions;
//		m_aabb.maxDimensions = maxDimensions;
//	}
//	
//	bool Box::TestIntersection(const glm::vec3& rayOrigin, const glm::vec3& rayDirection, HitInfo& hitInfo)
//	{
//		float hitDistance;
//		glm::vec3 hitNormal;
//		bool inside;
//
//		if(!RTMath::RayAABBIntersection(rayOrigin, rayDirection, m_aabb, hitNormal, hitDistance, inside)) return false;
//		
//		hitInfo.hitDistance = hitDistance;
//		hitInfo.hitNormal = hitNormal;
//		hitInfo.insideObject = inside;
//		return true;
//	}
//}

namespace RayTracer
{
	Box::Box(glm::mat4 transform, vec3 dimensions) : Geometry(transform), dimensions(dimensions)
	{
		float xDim = dimensions.x / 2.0f;
		float yDim = dimensions.y / 2.0f;
		float zDim = dimensions.z / 2.0f;

		minDimensions = { -xDim, -yDim, -zDim };
		maxDimensions = { xDim, yDim, zDim };
	}

	SurfaceInteraction Box::Sample(const vec2 & u) const
	{
		return SurfaceInteraction();
	}

	float Box::Area() const
	{
		return dimensions.x * dimensions.y * dimensions.z;
	}

	bool Box::TestIntersection(const Ray& ray, float &tHit, shared_ptr<SurfaceInteraction> interaction) const
	{
		vec3 hitNormal;
		bool inside;

		if (!RTMath::RayAABBIntersection(ray, minDimensions, maxDimensions, 
			hitNormal, tHit, inside)) return false;

		if (tHit > ray.tMax) return false;

		interaction->geometricNormal = hitNormal;

		return true;
	}
}