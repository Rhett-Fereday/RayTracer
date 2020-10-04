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
	Camera camera = RayTracer::Camera(500, 500, 45.0f, { 0,0.5,0 }, { 0,0,-3 });
	Scene scene(camera);

	

	// Create the materials to be used in the scene
	ConstMaterial bunnyMat; bunnyMat.albedo = { 1,1,1 }; bunnyMat.emissiveness = { 0.0f,0.0f,0.0f }; bunnyMat.reflectiveness = 0.0f;
	ConstMaterial amphoraMat; amphoraMat.albedo = { 0.955, 0.637, 0.538 }; amphoraMat.emissiveness = { 0.0f,0.0f,0.0f }; amphoraMat.reflectiveness = 0.0f;
	ConstMaterial teapotMat; teapotMat.albedo = { 0.972, 0.960, 0.915 }; teapotMat.emissiveness = { 0.0f,0.0f,0.0f }; teapotMat.reflectiveness = 0.0f;
	ConstMaterial floorMat1; floorMat1.albedo = { 1,0,0 }; floorMat1.emissiveness = { 0.0f,0.0f,0.0f }; floorMat1.reflectiveness = 0.0f;
	ConstMaterial floorMat2; floorMat2.albedo = { 0.1, 0.1, 0.1 }; floorMat2.emissiveness = { 0.0f,0.0f,0.0f }; floorMat2.reflectiveness = 0.0f;
	ConstMaterial backWallMat; backWallMat.albedo = { 1,1,1 }; backWallMat.emissiveness = { 0.0f,0.0f,0.0f }; backWallMat.reflectiveness = 0.0f;
	ConstMaterial areaLightMat; areaLightMat.albedo = { 1,1,1 }; areaLightMat.emissiveness = { 1,1,1 }; areaLightMat.reflectiveness = 0.0f;

	// Add sunlight to the scene
	DirectionalLight sunlight = DirectionalLight({ -0.5,-1,-1 });
	//scene.AddLight(&sunlight);

	PointLight plight = PointLight({ 1,1,1 }, 5.0f, { 0, 1, -3.5 });
	//scene.AddLight(&plight);

	// Add an area light
	glm::mat4 transform = glm::translate(glm::mat4(1.0f), { 0, 1, -3 });
	BoxLight boxLight = BoxLight(transform, { 1,1,1 }, &areaLightMat, { 1,1,1 }, 5.0f);
	scene.AddLight(&boxLight); 
	//scene.AddObject(&boxLight); // Area lights are objects and lights

	// Load the meshes for the scene
	//Mesh bunnyMesh = Mesh("bunny_very_hi.obj", 15);
	//Mesh amphoraMesh = Mesh("amphora.obj", 10);
	Mesh teapotMesh = Mesh("teapot.obj", 11);

	// Create the mesh instances and add them to the scene
	transform = glm::translate(glm::mat4(1.0f), { 0.15, 0.05, -3 });
	//transform = glm::rotate(transform, glm::radians(45.0f), { 0, 1, 0 });
	transform = glm::scale(transform, { 0.25,0.25,0.25 });
	//MeshInstance bunnyInstance = MeshInstance(&bunnyMesh, transform, &bunnyMat);
	//scene.AddObject(&bunnyInstance);

	transform = glm::translate(glm::mat4(1.0f), { 0, 0, -2.75 });
	transform = glm::rotate(transform, glm::radians(-90.0f), { 1,0,0 });
	transform = glm::rotate(transform, glm::radians(-25.0f), { 0,0,1 });
	transform = glm::scale(transform, { 0.025, 0.025, 0.025 });
	MeshInstance teapotInstance = MeshInstance(&teapotMesh, transform, &teapotMat);
	scene.AddObject(&teapotInstance);

	transform = glm::translate(glm::mat4(1.0f), { 0.75, 0, -2.75 });
	transform = glm::scale(transform, { 0.75, 0.75, 0.75 });
	//MeshInstance amphoraInstance = MeshInstance(&amphoraMesh, transform, &amphoraMat);
	//scene.AddObject(&amphoraInstance);

	// Add the floor and wall to the scene
	transform = glm::translate(glm::mat4(1.0f), { 0,-0.025,-2.5 });
	Box floor = Box(transform, &floorMat1, { 10,0.05,10 });
	scene.AddObject(&floor);

	// Add back wall
	transform = glm::translate(glm::mat4(1.0f), { 0,0,-5 });
	Box backWall = Box(transform, &backWallMat, { 10,10,0.05 });
	//scene.AddObject(&backWall);


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