#include "Scene.h"
#include "Camera.h"
#include "Sphere.h"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>
#include "Material.h"

int main(int argc, char* argv[])
{
	RayTracer::Camera camera = RayTracer::Camera(3840,2160);
	RayTracer::Scene scene(camera);

	glm::mat4 transform = glm::translate(glm::mat4(1.0f), glm::vec3(-1.5f, 0, -5));
	transform = glm::rotate(transform, glm::radians(45.0f), glm::vec3(1, 0, 0));

	RayTracer::Material mat = RayTracer::Material("C:\\Users\\gmfer\\Documents\\Local Repos\\RayTracer\\TestProgram\\earth.ppm");
	RayTracer::Sphere leftSphere = RayTracer::Sphere(transform, 0.5f, mat);

	transform = glm::translate(glm::mat4(1.0f), glm::vec3(1.5f, 0, -5));
	transform = glm::rotate(transform, glm::radians(-45.0f), glm::vec3(0, 1, 0));
	RayTracer::Sphere rightSphere = RayTracer::Sphere(transform, 0.5f, mat);

	transform = glm::translate(glm::mat4(1.0f), glm::vec3(0, -1000, -5));
	RayTracer::Sphere groundSphere = RayTracer::Sphere(transform, 999.0f, mat);
	
	transform = glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, -5));
	transform = glm::rotate(transform, glm::radians(-90.0f), glm::vec3(0, 1, 0));

	RayTracer::Sphere midSphere = RayTracer::Sphere(transform, 0.5f, mat);

	scene.AddSphere(leftSphere);
	scene.AddSphere(midSphere);
	scene.AddSphere(rightSphere);
	scene.AddSphere(groundSphere);

	scene.RenderScene();
	scene.SaveScene("TestImage.ppm");
}