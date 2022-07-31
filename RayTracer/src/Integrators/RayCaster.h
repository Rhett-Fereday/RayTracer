#pragma once

#include "../Core.h"
#include "Integrator.h"

namespace RayTracer
{
	class RT_API RayCaster : public Integrator
	{
	public:
		RayCaster();

	protected:
		virtual vec3 ComputeRayContribution(const Ray& ray, shared_ptr<Scene> scene, const shared_ptr<SampleGenerator> generator, const vector<int>& samplesPerLight, int pixelSample) override;
	};
}