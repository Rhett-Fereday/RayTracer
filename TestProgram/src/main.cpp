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
	Camera camera = RayTracer::Camera(500, 500);// , 90, { 0,5,0 }, { 0, -5, -17 });
	Scene scene(camera);

	// Create the lights for the scene
	RayTracer::SpotLight spotLight1 = RayTracer::SpotLight({ 1,1,1 }, 50.0f, { 2,5,-12 }, glm::vec3(0, -5, -17) - glm::vec3(2, 5, -12), 75.0f, 90.0f);
	RayTracer::SpotLight spotLight2 = RayTracer::SpotLight({ 1,1,1 }, 50.0f, { -2,5,-22 }, glm::vec3(0, -5, -17) - glm::vec3(-2, 5, -22), 75.0f, 90.0f);

	scene.AddLight(&spotLight1);
	scene.AddLight(&spotLight2);

	//DirectionalLight sunLight = DirectionalLight({ 0,-1,-1 }, { 1,1,1 });
	//scene.AddLight(&sunLight);

	// Create the materials that will be used in the scene
	RayTracer::ConstMaterial redMat, blueMat, whiteMat, goldMat, silverMat, copperMat, mirrorMat;
	redMat.albedo = { 1,0,0 }; redMat.reflectiveness = 0.0f;
	blueMat.albedo = { 0,0,1 }; blueMat.reflectiveness = 0.0f;
	whiteMat.albedo = { 1,1,1 }; whiteMat.reflectiveness = 0.0f;
	goldMat.albedo = { 1, 0.766, 0.336 }; goldMat.reflectiveness = 0.2f;
	silverMat.albedo = { 0.972, 0.960, 0.915 }; silverMat.reflectiveness = 0.0f;
	copperMat.albedo = { 0.955, 0.637, 0.538 }; copperMat.reflectiveness = 0.0f;// 0.1f;
	mirrorMat.albedo = { 1,1,1 }; mirrorMat.reflectiveness = 0.0f;

	// Add amphora
	glm::mat4 transform = glm::translate(glm::mat4(1.0f), glm::vec3(-1, -0.5, -3.75));
	/*Mesh amphoraMesh = Mesh("amphora.obj", 10);
	MeshInstance amphoraInstance1 = MeshInstance(&amphoraMesh, transform, &goldMat);
	scene.AddObject(&amphoraInstance1);*/

	// Add stanford bunny
	transform = glm::translate(glm::mat4(1.0f), glm::vec3(0, -5, -17));
	//transform = glm::rotate(transform, glm::radians(-90.0f), { 0,1,0 });
	transform = glm::scale(transform, { 0.1,0.1,0.1 });
	
	Mesh bunnyMesh = Mesh("car.obj", 11);
	MeshInstance bunnyInstance1 = MeshInstance(&bunnyMesh, transform, &silverMat);
	scene.AddObject(&bunnyInstance1);

	// Create Room
	transform = glm::translate(glm::mat4(1.0f), glm::vec3(0, -5.05, 0));
	RayTracer::Box floor = RayTracer::Box(transform, &redMat, glm::vec3(100, 0.05, 100));

	transform = glm::translate(glm::mat4(1.0f), glm::vec3(15, 0, 0));
	transform = glm::rotate(transform, glm::radians(90.0f), glm::vec3(0, 0, 1));
	//transform = glm::rotate(transform, glm::radians(45.0f), glm::vec3(0, 0, 1));
	RayTracer::Box rightWall = RayTracer::Box(transform, &mirrorMat, glm::vec3(100, 0.05, 100));

	transform = glm::translate(glm::mat4(1.0f), glm::vec3(-15, 0, 0));
	transform = glm::rotate(transform, glm::radians(90.0f), glm::vec3(0, 0, 1));
	//transform = glm::rotate(transform, glm::radians(-45.0f), glm::vec3(1, 0, 0));
	RayTracer::Box leftWall = Box(transform, &mirrorMat, glm::vec3(100, 0.05, 100));

	transform = glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, -35));
	transform = glm::rotate(transform, glm::radians(90.0f), glm::vec3(1, 0, 0));
	//transform = glm::rotate(transform, glm::radians(45.0f), glm::vec3(0, 0, 1));
	RayTracer::Box backWall = RayTracer::Box(transform, &mirrorMat, glm::vec3(100, 0.05, 100));

	transform = glm::translate(glm::mat4(1.0f), glm::vec3(0, 10, 0));
	RayTracer::Box ceiling = RayTracer::Box(transform, &mirrorMat, glm::vec3(100, 0.05, 100));
	
	scene.AddObject(&floor);
	scene.AddObject(&rightWall);
	scene.AddObject(&leftWall);
	scene.AddObject(&backWall);
	scene.AddObject(&ceiling);

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