#include "Scene.h"
#include "Camera.h"
#include "Sphere.h"

int main(int argc, char* argv[])
{
	RayTracer::Camera camera = RayTracer::Camera();
	RayTracer::Scene scene(camera);

	RayTracer::Sphere sphere = RayTracer::Sphere(glm::mat4(), 1.0f, { 1,0,0 });

	scene.AddSphere(sphere);

	scene.RenderScene();
	scene.SaveScene("TestImage.ppm");
}