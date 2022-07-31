#pragma once

#include "Geometry.h"
#include "../../Core.h"

namespace RayTracer
{
	class RT_API Plane : public Geometry
	{
	public:
		Plane(glm::mat4 transform, float length, float width);
		Plane(glm::mat4 transform, float dimension);

		virtual SurfaceInteraction Sample(const vec2& u) const override;

		virtual float Area() const override;

	protected:
		virtual bool TestIntersection(const Ray& ray, float& tHit, shared_ptr<SurfaceInteraction> interaction) const override;

	private:
		float length, width;
	};
}