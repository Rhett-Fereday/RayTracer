#pragma once

#include "../../Core.h"
#include "Light.h"
#include "../Geometry/Geometry.h"

namespace RayTracer
{
	class RT_API AreaLight : public Light
	{
	public:
		AreaLight(glm::mat4 transform, shared_ptr<Geometry> geometry, vec3 color, float totalPower, int nSamples = 1);

		virtual vec3 L(const SurfaceInteraction& interaction, const vec3& w) const override;
		virtual vec3 SampleLi(const Interaction& interaction, const vec2& u, vec3& wi, float &pdf, float& sampleDistance) const override;
		virtual float PdfLi(const Interaction& interaction, const vec3& wi) const override;

		virtual vec3 Power() const override;

		shared_ptr<Geometry> geometry;
	};
}