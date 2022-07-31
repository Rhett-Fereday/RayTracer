#pragma once

#include "../../Core.h"
#include "Geometry.h"

namespace RayTracer
{
	class RT_API Box : public Geometry
	{
	public:
		Box(glm::mat4 transform, vec3 dimensions);
		
		virtual SurfaceInteraction Sample(const vec2& u) const override;

		virtual float Area() const override;

	protected:
		virtual bool TestIntersection(const Ray& ray, float& tHit, shared_ptr<SurfaceInteraction> interaction) const override;

	private:
		vec3 dimensions, minDimensions, maxDimensions;
	};
}