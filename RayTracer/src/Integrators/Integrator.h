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
		struct RenderTile
		{
			int startX = 0;
			int startY = 0;
		};

	public:
		Integrator(int maxBounces = 1) : maxBounces(maxBounces) { };

		void Render(shared_ptr<Scene> scene, shared_ptr<Camera> camera, shared_ptr<SampleGenerator> generator, int numThreads);
	
	protected:
		int maxBounces;
		virtual vec3 ComputeRayContribution(const Ray& ray, shared_ptr<Scene> scene, const shared_ptr<SampleGenerator> generator, const vector<int>& samplesPerLight, int pixelSample) = 0;
		vec3 UniformSampleOneLight(const shared_ptr<SurfaceInteraction> interaction, const shared_ptr<Scene> scene, const shared_ptr<SampleGenerator> generator, const vector<int>& samplesPerLight, int pixelSample) const;
		vec3 UniformSampleAllLights(const shared_ptr<SurfaceInteraction> interaction, const shared_ptr<Scene> scene, const shared_ptr<SampleGenerator> generator, const vector<int>& samplesPerLight, int pixelSample) const;
		vec3 EstimateDirectLighting(const shared_ptr<SurfaceInteraction> interaction, const shared_ptr<Scene> scene, const shared_ptr<Light> light, const vec2& u) const;
	
	private:
		int tileDim = 16;
		std::queue<RenderTile> renderQueue;
		std::mutex renderMutex;
		void ThreadRender(shared_ptr<Scene> scene, shared_ptr<Camera> camera, shared_ptr<SampleGenerator> generator);
	};
}