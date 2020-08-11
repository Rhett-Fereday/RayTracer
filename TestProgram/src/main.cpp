#include "Scene.h"
#include "Camera.h"

int main(int argc, char* argv[])
{
	RayTracer::Camera camera = RayTracer::Camera();
	RayTracer::Scene scene(camera);

	scene.RenderScene();
	scene.SaveScene("TestImage.png");
}