#include "Disk.h"
#include "../../Interactions/SurfaceInteraction.h"
#include "../../RTMath.h"

namespace RayTracer
{
	Disk::Disk(glm::mat4 transform, float radius)
		: Geometry(transform), radius(radius)
	{
	}

	SurfaceInteraction Disk::Sample(const vec2& u) const
	{
		SurfaceInteraction interaction;

		auto objP = glm::vec4(radius * RTMath::ConcentricSampleDisk(u), 0, 1);
		auto worldP = transform * objP;
		auto worldN = inverseTransposeTransform * glm::vec4(0, 0, 1, 0);

		interaction.p = vec3(worldP.x, worldP.y, worldP.z);
		interaction.geometricNormal = vec3(worldN.x, worldN.y, worldN.z);

		return interaction;
	}

	float Disk::Area() const
	{
		return pi * radius * radius;
	}

	bool Disk::TestIntersection(const Ray& ray, float& tHit, shared_ptr<SurfaceInteraction> interaction) const
	{
		if (!RTMath::RayDiskIntersection(ray, radius, tHit)) return false;

		if (tHit > ray.tMax) return false;

		interaction->geometricNormal = { 0,0,1 };

		return true;
	}


}