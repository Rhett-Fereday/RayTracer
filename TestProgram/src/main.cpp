#include "Scene.h"
#include "Camera.h"
#include "Sphere.h"
#include "Box.h"
#include "Mesh.h"
#include "MeshInstance.h"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>
#include "ConstMaterial.h"
#include "DirectionalLight.h"
#include "PointLight.h"
#include "SpotLight.h"
#include <time.h>
#include <iostream>

using namespace RayTracer;

int main(int argc, char* argv[])
{
	// Create the camera with 1080p resolution and create a scene with it
	Camera camera = RayTracer::Camera(300,300);
	Scene scene(camera);

	// Create the lights for the scene
	/*SpotLight spotLight1 = SpotLight({ 1,1,1 }, 100.0f, {0,0,0}, { 0,0,-1 }, 10.0f, 12.0f);
	scene.AddLight(&spotLight1);*/
	DirectionalLight sunLight = DirectionalLight({ 0.5,-1,-1 }, { 1,1,1 });
	scene.AddLight(&sunLight);

	// Create a material
	ConstMaterial whiteMat; whiteMat.albedo = { 1,1,1 }; whiteMat.reflectiveness = 0.0f;
	ConstMaterial redMat; redMat.albedo = { 1,0,0 }; redMat.reflectiveness = 0.0f;

	// Add a triangular mesh to the scene

	glm::mat4 transform = glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, -50));	
	transform = glm::rotate(transform, glm::radians(-35.0f), { 0,1,0 });
	transform = glm::rotate(transform, glm::radians(-60.0f), { 1,0,0 });
	
	Mesh mesh = Mesh("teapot.obj");
	MeshInstance instance1 = MeshInstance(&mesh, transform, &whiteMat);
	scene.AddObject(&instance1);

	transform = glm::translate(transform, glm::vec3(0, -25, 0));
	MeshInstance instance2 = MeshInstance(&mesh, transform, &redMat);
	scene.AddObject(&instance2);

	// Render and save the image. The image saves in the TestProgram folder (when I run the project that's where VS puts it at least)

	time_t startTime = time(0);
	scene.RenderScene();
	time_t endTime = time(0);

	time_t deltaTime = endTime - startTime;

	scene.SaveScene("TestImage.ppm");

	float hours = deltaTime / (60.0f * 60.0f);
	int wholeHours = int(hours);

	float minutes = (hours - wholeHours) * 60.0f;
	int wholeMinutes = int(minutes);

	float seconds = (minutes - wholeMinutes) * 60.0f;
	int wholeSeconds = int(seconds);


	std::cout << "Time to render: " << wholeHours << " hours, " << wholeMinutes << " minutes, " << wholeSeconds << " seconds." << std::endl;
}