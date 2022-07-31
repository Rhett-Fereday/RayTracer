#pragma once

#include "../../Core.h"
#include "Geometry.h"

namespace RayTracer
{
	class RT_API Sphere : public Geometry
	{
	public:
		Sphere(float radius = 1.0f, glm::mat4 transform = glm::mat4(1.0));

	protected:		
		virtual bool TestIntersection(const Ray& ray, float& tHit, shared_ptr<SurfaceInteraction> interaction) const override;
		virtual SurfaceInteraction Sample(const vec2& u) const override;

		virtual float Area() const override;

	private:
		float radius;

	};
}