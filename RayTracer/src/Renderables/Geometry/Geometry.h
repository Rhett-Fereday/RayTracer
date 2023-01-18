#pragma once

#include "../../Core.h"
#include "glm/glm.hpp"

namespace RayTracer
{
	class SurfaceInteraction;
	class Interaction;
	class Ray;

	class RT_API Geometry
	{
	public:
		Geometry(glm::mat4 transform);
		bool Intersects(const Ray& ray, float& tHit, shared_ptr<SurfaceInteraction> interaction) const;
		
		virtual SurfaceInteraction Sample(const vec2& u) const = 0;
		virtual SurfaceInteraction Sample(const Interaction& interaction, const vec2& u, float& pdf) const;		

		virtual float Pdf(const Interaction& interaction) const;
		virtual float Pdf(const Interaction& interaction, const vec3& wi) const;

		virtual float Area() const = 0;

	protected:
		glm::mat4 transform, inverseTransform, inverseTransposeTransform;
		bool transformIsIdentity;

		virtual bool TestIntersection(const Ray& ray, float& tHit, shared_ptr<SurfaceInteraction> interaction) const = 0;
	};
}