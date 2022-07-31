#pragma once

#include "Integrator.h"
#include "../Core.h"

namespace RayTracer
{
	class RT_API PathTracer : public Integrator
	{
	public:
		PathTracer(int maxBounces = 4);

	protected:
		virtual vec3 ComputeRayContribution(const Ray& ray, shared_ptr<Scene> scene, const shared_ptr<SampleGenerator> generator, const vector<int>& samplesPerLight, int pixelSample) override;
	};
}