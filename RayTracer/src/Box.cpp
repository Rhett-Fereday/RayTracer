#include "Box.h"
#include "RTMath.h"

namespace RayTracer
{
	Box::Box(glm::mat4 transform, ConstMaterial* material, glm::vec3 dimensions) : Object(transform, material)
	{
		float xDim = dimensions.x / 2.0f;
		float yDim = dimensions.y / 2.0f;
		float zDim = dimensions.z / 2.0f;

		m_aabb.minDimensions = { -xDim, -yDim, -zDim };
		m_aabb.maxDimensions = { xDim, yDim, zDim };
	}

	Box::Box(glm::mat4 transform, ConstMaterial * material, glm::vec3 minDimensions, glm::vec3 maxDimensions) : Object(transform, material)
	{
		m_aabb.minDimensions = minDimensions;
		m_aabb.maxDimensions = maxDimensions;
	}
	
	bool Box::TestIntersection(const glm::vec3& rayOrigin, const glm::vec3& rayDirection, HitInfo& hitInfo)
	{
		float hitDistance;
		glm::vec3 hitNormal;

		if(!RTMath::RayAABBIntersection(rayOrigin, rayDirection, m_aabb, hitNormal, hitDistance)) return false;
		
		hitInfo.hitDistance = hitDistance;
		hitInfo.hitNormal = hitNormal;
		return true;
	}
}