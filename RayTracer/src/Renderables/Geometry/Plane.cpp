#include "Plane.h"
#include "../../Interactions/SurfaceInteraction.h"
#include "../../RTMath.h"

namespace RayTracer
{
	Plane::Plane(glm::mat4 transform, float length, float width)
		: Geometry(transform), length(length), width(width)
	{
	}

	Plane::Plane(glm::mat4 transform, float dimension) 
		: Plane(transform, dimension, dimension)
	{
	}

	SurfaceInteraction Plane::Sample(const vec2& u) const
	{
		// Remap u to [-1,1]
		vec2 uOffset = 2.0f * u - vec2(1, 1);
		auto halfWidth = width * 0.5f;
		auto halfLength = length * 0.5f;

		auto worldP = transform * glm::vec4(halfWidth * uOffset.x, halfLength * uOffset.y, 0, 1);
		auto worldN = inverseTransposeTransform * glm::vec4(0, 0, 1, 0);

		SurfaceInteraction interaction;
		interaction.p = { worldP.x, worldP.y, worldP.z };
		interaction.geometricNormal = { worldN.x, worldN.y, worldN.z };

		return interaction;
	}

	float Plane::Area() const
	{
		return length * width;
	}

	bool Plane::TestIntersection(const Ray& ray, float& tHit, shared_ptr<SurfaceInteraction> interaction) const
	{
		if (!RTMath::RayPlaneIntersection(ray, width, length, tHit)) return false;

		if (tHit > ray.tMax) return false;

		interaction->geometricNormal = { 0,0,1 };

		return true;
	}
}