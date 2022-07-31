#pragma once

#include "../Core.h"
#include "../Camera/Camera.h"
#include "../Ray.h"
#include "../Scene.h"
#include "../SampleGenerators/SampleGenerator.h"

namespace RayTracer
{
	class RT_API Integrator
	{
	public:
		Integrator(int maxBounces = 1) : maxBounces(maxBounces) { };

		void Render(shared_ptr<Scene> scene, shared_ptr<Camera> camera, shared_ptr<SampleGenerator> generator, float epsilon = 1e-6);
	
	protected:
		int maxBounces;
		virtual vec3 ComputeRayContribution(const Ray& ray, shared_ptr<Scene> scene, const shared_ptr<SampleGenerator> generator, const vector<int>& samplesPerLight, int pixelSample) = 0;
		vec3 UniformSampleAllLights(const shared_ptr<SurfaceInteraction> interaction, const shared_ptr<Scene> scene, const shared_ptr<SampleGenerator> generator, const vector<int>& samplesPerLight, int pixelSample) const;
		vec3 EstimateDirectLighting(const shared_ptr<SurfaceInteraction> interaction, const shared_ptr<Scene> scene, const shared_ptr<Light> light, const vec2& u) const;
	};
}