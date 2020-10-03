#include "Sphere.h"
#include "RTMath.h"
#include "glm/glm.hpp"

namespace RayTracer
{
    Sphere::Sphere(glm::mat4 transform, ConstMaterial* material) : Object(transform, material)
    {

    }

    // Modified from scratchapixel.com to support my program structure
    bool Sphere::TestIntersection(const glm::vec3& rayOrigin, const glm::vec3& rayDirection, HitInfo& hitInfo)
    {
		glm::vec3 hitNormal;
		float hitDistance;

		if (!RTMath::RaySphereIntersection(rayOrigin, rayDirection, 1.0f, hitNormal, hitDistance)) return false;

		hitInfo.hitDistance = hitDistance;
		hitInfo.hitNormal = hitNormal;
		return true;
    }
}