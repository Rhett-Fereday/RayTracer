#include "Scene.h"
#include "Camera.h"
#include "Sphere.h"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>

int main(int argc, char* argv[])
{
	RayTracer::Camera camera = RayTracer::Camera();
	RayTracer::Scene scene(camera);

	glm::mat4 transform = glm::translate(glm::mat4(1.0f), glm::vec3(-1.5f, 0, -5));
	RayTracer::Sphere leftSphere = RayTracer::Sphere(transform, 0.5f, { 1,0,0 });

	transform = glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, -5));
	RayTracer::Sphere midSphere = RayTracer::Sphere(transform, 0.5f, { 0,1,0 });

	transform = glm::translate(glm::mat4(1.0f), glm::vec3(1.5f, 0, -5));
	RayTracer::Sphere rightSphere = RayTracer::Sphere(transform, 0.5f, { 0,0,1 });

	transform = glm::translate(glm::mat4(1.0f), glm::vec3(0, -1000, -5));
	RayTracer::Sphere groundSphere = RayTracer::Sphere(transform, 999.0f, { 1,1,1 });

	scene.AddSphere(leftSphere);
	scene.AddSphere(midSphere);
	scene.AddSphere(rightSphere);
	scene.AddSphere(groundSphere);

	scene.RenderScene();
	scene.SaveScene("TestImage.ppm");
}