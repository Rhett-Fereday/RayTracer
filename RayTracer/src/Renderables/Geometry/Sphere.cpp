#include "Sphere.h"
#include "../../Interactions/SurfaceInteraction.h"
#include "../../RTMath.h"
//#include "Sphere.h"
//#include "../RTMath.h"
//#include "glm/glm.hpp"
//
//namespace RayTracer
//{
//    Sphere::Sphere(glm::mat4 transform, ConstMaterial* material) : Object(transform, material)
//    {
//
//    }
//
//    // Modified from scratchapixel.com to support my program structure
//    bool Sphere::TestIntersection(const glm::vec3& rayOrigin, const glm::vec3& rayDirection, HitInfo& hitInfo)
//    {
//		glm::vec3 hitNormal;
//		float hitDistance;
//		bool inside;
//
//		if (!RTMath::RaySphereIntersection(rayOrigin, rayDirection, 1.0f, hitNormal, hitDistance, inside)) return false;
//
//		hitInfo.hitDistance = hitDistance;
//		hitInfo.hitNormal = hitNormal;
//		hitInfo.insideObject = inside;
//		return true;
//    }
//}

namespace RayTracer
{
	Sphere::Sphere(float radius, glm::mat4 transform) 
		: radius(radius), Geometry(transform)
	{
	}	

	bool Sphere::TestIntersection(const Ray& ray, float& tHit, shared_ptr<SurfaceInteraction> interaction) const
	{
		glm::vec3 hitNormal;
		bool inside;

		if (!RTMath::RaySphereIntersection(ray, radius, hitNormal, tHit, inside)) return false;

		if (tHit > ray.tMax) return false;

		interaction->geometricNormal = hitNormal;

		return true;
	}

	SurfaceInteraction Sphere::Sample(const vec2 & u) const
	{
		auto p = radius * RTMath::UniformSampleSphere(u);

		auto worldP = transform * glm::vec4(p, 1);
		auto center = transform * glm::vec4(0,0,0,1);

		SurfaceInteraction interaction;
		interaction.p = { worldP.x, worldP.y, worldP.z };
		interaction.geometricNormal = glm::normalize(vec3(worldP.x, worldP.y, worldP.z) 
			- vec3(center.x, center.y, center.z));
		//interaction.geometricNormal = glm::normalize(vec3(worldP.x, worldP.y, worldP.z));

		return interaction;
	}

	float Sphere::Area() const
	{
		return 4.0f * pi * radius * radius;
	}
}