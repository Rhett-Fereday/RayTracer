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
#include <time.h>
#include <iostream>

using namespace RayTracer;

int main(int argc, char* argv[])
{
	// Create the camera and create a scene with it

	Camera camera = RayTracer::Camera(640, 480, 45.0f, { 0,0.75,2 }, { 0,0.75,0 }, 0.0001);
	Scene scene(camera);
	

	// Create the materials to be used in the scene
	ConstMaterial teapotMat1; teapotMat1.albedo = { 0.972, 0.960, 0.915 }; teapotMat1.emissiveColor = { 0,0,0 }; teapotMat1.reflectiveness = 0.0f; teapotMat1.isTransparent = true; teapotMat1.refractiveIndex = 1.5f; teapotMat1.reflectionLobeAngle = 0.0f; teapotMat1.transmissionLobeAngle = 3.0f;
	ConstMaterial teapotMat2; teapotMat2.albedo = { 67.0f / 255.0f, 94.0f / 255.0f, 82.0f / 255.0f }; teapotMat2.emissiveColor = { 0,0,0 }; teapotMat2.reflectiveness = 0.1f; teapotMat2.isTransparent = false;
	ConstMaterial teapotMat3; teapotMat3.albedo = { 16.0f / 255.0f, 32.0f / 255.0f, 75.0f / 255.0f }; teapotMat3.emissiveColor = { 0,0,0 }; teapotMat3.reflectiveness = 0.1f; teapotMat3.isTransparent = false;
	ConstMaterial greenWallMat; greenWallMat.albedo = { 86.0f / 255.0f,125.0f / 255.0f,70.0f / 255.0f }; greenWallMat.emissiveColor = { 0.0f,0.0f,0.0f }; greenWallMat.reflectiveness = 0.0f; greenWallMat.isTransparent = false;
	ConstMaterial whiteMat; whiteMat.albedo = { 1,1,1 }; whiteMat.emissiveColor = { 0.0f,0.0f,0.0f }; whiteMat.reflectiveness = 0.0f; whiteMat.isTransparent = false;
	ConstMaterial redWallMat; redWallMat.albedo = { 0.545,0,0 }; redWallMat.emissiveColor = { 0,0,0 }; redWallMat.reflectiveness = 0.0f; redWallMat.isTransparent = false;
	ConstMaterial backWallMat; backWallMat.albedo = { 0.8,0,0 }; backWallMat.emissiveColor = { 0,0,0 }; backWallMat.reflectiveness = 0.0f; backWallMat.isTransparent = false;
	ConstMaterial areaLightMat; areaLightMat.albedo = { 1,1,1 }; areaLightMat.emissiveStrength = 1.0f;
	ConstMaterial glowCubeMat; glowCubeMat.albedo = { 0,1,0 }; glowCubeMat.emissiveColor = { 0,1,0 };


	// Add area lights
	glm::mat4 transform = glm::translate(glm::mat4(1.0f), { 0, 1.48, 0 });
	BoxLight boxLight1 = BoxLight(transform, { 0.25,0.01,0.25 }, &areaLightMat, { 1,1,1 }, 1.0f);
	//scene.AddLight(&boxLight1);

	transform = glm::translate(glm::mat4(1.0f), { 0, 1.4, 0 });
	Box fakeLight1 = Box(transform, &areaLightMat, { 0.25, 0.01, 0.25 });
	scene.AddObject(&fakeLight1);

	PointLight pointLight = PointLight({ 1,1,1 }, 10, { 0, 0.75, 0 });
	//scene.AddLight(&pointLight);

	//DirectionalLight sunLight = DirectionalLight({ 0,-1,-1 }, { 1,1,1 }, 2.0f);
	//scene.AddLight(&sunLight);

	// Load the meshes for the scene
	//Mesh teapotMesh = Mesh("teapot.obj", 11);
	//Mesh bunnyMesh = Mesh("bunny_very_hi.obj", 15);
	//Mesh teacupMesh = Mesh("teacup.obj", 14);
	//Mesh amphoraMesh = Mesh("amphora.obj", 10);

	transform = glm::translate(glm::mat4(1.0f), { 0, 0, -3 });
	//transform = glm::rotate(transform, glm::radians(-90.0f), { 1,0,0 });
	//transform = glm::rotate(transform, glm::radians(-35.0f), { 0,0,1 });
	transform = glm::scale(transform, { 0.05, 0.05, 0.05 });
	//MeshInstance teacupInstance = MeshInstance(&teacupMesh, transform, &teapotMat1);
	//scene.AddObject(&teacupInstance);

	transform = glm::translate(glm::mat4(1.0f), { 0.1, 0.05, -3 });
	//transform = glm::rotate(transform, glm::radians(-90.0f), { 1,0,0 });
	//transform = glm::rotate(transform, glm::radians(-35.0f), { 0,0,1 });
	transform = glm::scale(transform, { 0.2, 0.2, 0.2 });
	//MeshInstance bunnyInstance = MeshInstance(&bunnyMesh, transform, &teapotMat1);
	//scene.AddObject(&bunnyInstance);

	transform = glm::translate(glm::mat4(1.0f), { 0.75, 0, -3.75 });
	transform = glm::rotate(transform, glm::radians(-90.0f), { 1,0,0 });
	transform = glm::rotate(transform, glm::radians(-25.0f), { 0,0,1 });
	transform = glm::scale(transform, { 0.025, 0.025, 0.025 });
	//MeshInstance teapotInstance2 = MeshInstance(&teapotMesh, transform, &teapotMat2);
	//scene.AddObject(&teapotInstance2);

	transform = glm::translate(glm::mat4(1.0f), { 0, 0, -3.75 });
	transform = glm::rotate(transform, glm::radians(-90.0f), { 1,0,0 });
	transform = glm::rotate(transform, glm::radians(-155.0f), { 0,0,1 });
	transform = glm::scale(transform, { 0.025, 0.025, 0.025 });
	//MeshInstance teapotInstance3 = MeshInstance(&teapotMesh, transform, &teapotMat3);
	//scene.AddObject(&teapotInstance3);

	// Add a test sphere
	transform = glm::translate(glm::mat4(1.0f), { 0,0.5,-10 });
	//transform = glm::scale(transform, { 0.25, 0.25, 0.25 });
	Sphere sphere = Sphere(transform, &teapotMat1);
	//scene.AddObject(&sphere);

	// Add the floor and ceiling
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