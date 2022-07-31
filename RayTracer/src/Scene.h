#pragma once

#include "Core.h"
#include "Ray.h"
#include "Interactions/SurfaceInteraction.h"
#include "Renderables/Renderable.h"
#include "Renderables/Lights/Light.h"

namespace RayTracer
{
	class RT_API Scene
	{
	public:
		Scene(vec3 backGroundColor = { 0,0,0 });
		void AddRenderable(shared_ptr<Renderable> renderable);
		void AddLight(shared_ptr<Light> light);
		bool Intersects(const Ray& ray, shared_ptr<SurfaceInteraction> interaction);
		bool Intersects(const Ray& ray);
		//void AddLight(Light* light);
		//void RenderScene();
		//void SaveScene(const char* filename);

	//private:
		vector<shared_ptr<Renderable>> renderables;
		vector<shared_ptr<Light>> lights;
		vec3 backGroundColor;
	};
}