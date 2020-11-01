#include "Scene.h"
#include "Camera.h"
#include "Objects/Sphere.h"
#include "Objects/Box.h"
#include "Objects/Mesh.h"
#include "Objects/MeshInstance.h"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>
#include "ConstMaterial.h"
#include "Lights/DirectionalLight.h"
#include "Lights/PointLight.h"
#include "Lights/SpotLight.h"
#include "Lights/BoxLight.h"
#include "PostProcesses/NaiveReinhard.h"
#include <time.h>
#include <iostream>

using namespace RayTracer;

int main(int argc, char* argv[])
{
	// Create the camera and create a scene with it

	Camera camera = RayTracer::Camera(640, 480, 45.0f, { 0,0.75,2 }, { 0,0.75,0 }, 0.0001);

	NaiveReinhard toneMapper = NaiveReinhard();
	camera.AddPostProcess(&toneMapper);

	Scene scene(&camera);
	

	// Create the materials to be used in the scene
	ConstMaterial greenWallMat; greenWallMat.albedo = { 86.0f / 255.0f, 125.0f / 255.0f, 70.0f / 255.0f };
	ConstMaterial whiteMat; whiteMat.albedo = { 1,1,1 };
	ConstMaterial redWallMat; redWallMat.albedo = { 0.545f,0.0f,0.0f };
	ConstMaterial areaLightMat; areaLightMat.albedo = { 1,1,1 }; areaLightMat.emissiveStrength = 200.0f;

	glm::mat4 transform = glm::translate(glm::mat4(1.0f), { 0, 1.5, 0 });
	Box fakeLight1 = Box(transform, &areaLightMat, { 0.5, 0.0001, 0.5 });
	scene.AddObject(&fakeLight1);

	PointLight pointLight = PointLight({ 1,1,1 }, 5, { 0, 1.49, 0 });
	//scene.AddLight(&pointLight);

	// Load the meshes for the scene
	//Mesh teapotMesh = Mesh("teapot.obj", 11);
	//Mesh bunnyMesh = Mesh("bunny_very_hi.obj", 15);
	//Mesh teacupMesh = Mesh("teacup.obj", 14);
	//Mesh amphoraMesh = Mesh("amphora.obj", 10);

	// Construct the Cornell Box
	transform = glm::translate(glm::mat4(1.0f), { 0,-0.025,0 });
	Box floor = Box(transform, &whiteMat, { 2,0.05,2 });
	scene.AddObject(&floor);

	transform = glm::translate(glm::mat4(1.0f), { 0, 1.525, 0 });
	Box ceiling = Box(transform, &whiteMat, { 2, 0.05, 2 });
	scene.AddObject(&ceiling);

	transform = glm::translate(glm::mat4(1.0f), { -1.025,1,0 });
	Box leftWall = Box(transform, &redWallMat, { 0.05, 2, 2 });
	scene.AddObject(&leftWall);

	transform = glm::translate(glm::mat4(1.0f), { 1.025,1,0 });
	Box rightWall = Box(transform, &greenWallMat, { 0.05,2,2 });
	scene.AddObject(&rightWall);

	transform = glm::translate(glm::mat4(1.0f), { 0,1,-1.025 });
	Box backWall = Box(transform, &whiteMat, { 2,2,0.05 });
	scene.AddObject(&backWall);

	transform = glm::translate(glm::mat4(1.0f), { -0.45, 0.5, -0.3 });
	transform = glm::rotate(transform, glm::radians(30.0f), { 0,1,0 });
	Box leftBox = Box(transform, &whiteMat, { 0.4, 1, 0.4 });
	scene.AddObject(&leftBox);

	transform = glm::translate(glm::mat4(1.0f), { 0.5, 0.25, -0.25 });
	transform = glm::rotate(transform, glm::radians(25.0f), { 0,1,0 });
	Box rightBox = Box(transform, &whiteMat, { 0.3, 0.5, 0.3 });
	scene.AddObject(&rightBox);


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
	system("PAUSE");
}