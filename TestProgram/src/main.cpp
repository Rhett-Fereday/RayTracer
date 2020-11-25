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
#include "Lights/RectLight.h"
#include "PostProcesses/PostProcessGroup.h"
#include "PostProcesses/ModifiedReinhard.h"
#include "PostProcesses/GaussianBlur.h"
#include "PostProcesses/BilateralFilter.h"
#include "PostProcesses/ExtendedBilateralFilter.h"
#include "PostProcesses/GammaCorrection.h"
#include <time.h>
#include <iostream>

using namespace RayTracer;

int main(int argc, char* argv[])
{
	// Create the camera and create a scene with it

	Camera camera = RayTracer::Camera(500, 500, 45.0f, { 0,0.65,2 }, { 0,0.65,0 }, 0.0001);

	PostProcessGroup modifiedReinhardGCGroup = PostProcessGroup("_MR+GC");
	modifiedReinhardGCGroup.AddPostProcess(new ModifiedReinhard());
	modifiedReinhardGCGroup.AddPostProcess(new GammaCorrection());
	camera.AddPostProcessGroup(&modifiedReinhardGCGroup);

	/*PostProcessGroup gaussianGroup = PostProcessGroup("_GB");
	gaussianGroup.AddPostProcess(new GaussianBlur(8.0f));
	gaussianGroup.AddPostProcess(new ModifiedReinhard());
	gaussianGroup.AddPostProcess(new GammaCorrection());
	camera.AddPostProcessGroup(&gaussianGroup);

	PostProcessGroup bilateralFilterGroup = PostProcessGroup("_BF");
	bilateralFilterGroup.AddPostProcess(new BilateralFilter(8.0f, 0.075f));
	bilateralFilterGroup.AddPostProcess(new ModifiedReinhard());
	bilateralFilterGroup.AddPostProcess(new GammaCorrection());
	camera.AddPostProcessGroup(&bilateralFilterGroup);*/

	PostProcessGroup extendedBilateralFilterGroup1 = PostProcessGroup("_EBF");
	extendedBilateralFilterGroup1.AddPostProcess(new ExtendedBilateralFilter(8.0f, 0.075f, 0.05f, 0.05f, 0.05f));
	extendedBilateralFilterGroup1.AddPostProcess(new ModifiedReinhard());
	extendedBilateralFilterGroup1.AddPostProcess(new GammaCorrection());
	camera.AddPostProcessGroup(&extendedBilateralFilterGroup1);

	Scene scene(&camera);
	
	// Create the materials to be used in the scene
	ConstMaterial greenWallMat; greenWallMat.albedo = { 86.0f / 255.0f, 125.0f / 255.0f, 70.0f / 255.0f };
	ConstMaterial whiteMat; whiteMat.albedo = { 1,1,1 };
	ConstMaterial redWallMat; redWallMat.albedo = { 0.545f,0.0f,0.0f };
	ConstMaterial yellowAreaLightMat; yellowAreaLightMat.albedo = { 1,214.0f / 255.0f,170.0f / 255.0f }; yellowAreaLightMat.emissiveStrength = 30.0f;
	ConstMaterial whiteAreaLightMat; whiteAreaLightMat.albedo = { 1,1,1 }; whiteAreaLightMat.emissiveStrength = 20.0f;
	ConstMaterial mintGreenMat; mintGreenMat.albedo = { 67.0f / 255.0f, 94.0f / 255.0f, 82.0f / 255.0f };
	ConstMaterial blueMat; blueMat.albedo = { 16.0f / 255.0f, 32.0f / 255.0f, 75.0f / 255.0f };
	ConstMaterial blackMat; blackMat.albedo = { 0,0,0 };
	ConstMaterial goldMat; goldMat.albedo = { 1.00, 0.71, 0.29 }; goldMat.isMetal = true; goldMat.roughness = 0.5f;
	ConstMaterial silverMat; silverMat.albedo = { 0.95, 0.93, 0.88 }; silverMat.isMetal = true; silverMat.roughness = 0.3f;
	ConstMaterial whiteShinyMat; whiteShinyMat.roughness = 0.2;

	PointLight pointLight = PointLight({ 1,1,1 }, 1, { 0, 0.65, 0 });
	//scene.AddLight(&pointLight);

	glm::mat4 transform = glm::translate(glm::mat4(1.0f), { -0.5, 1.299, 0 });
	transform = glm::rotate(transform, glm::radians(90.0f), { 1,0,0 });
	RectLight rectLight1 = RectLight(transform, 0.1f, 0.1f, &yellowAreaLightMat);
	scene.AddObject(&rectLight1);
	scene.AddLight(&rectLight1);

	transform = glm::translate(glm::mat4(1.0f), { 0.5, 1.299, 0 });
	transform = glm::rotate(transform, glm::radians(90.0f), { 1,0,0 });
	RectLight rectLight2 = RectLight(transform, 0.1f, 0.1f, &yellowAreaLightMat);
	scene.AddObject(&rectLight2);
	scene.AddLight(&rectLight2);

	// Load the meshes for the scene
	Mesh teapotMesh = Mesh("teapot.obj", 11);
	//Mesh bunnyMesh = Mesh("bunny_very_hi.obj", 15);
	//Mesh teacupMesh = Mesh("teacup.obj", 14);
	//Mesh amphoraMesh = Mesh("amphora.obj", 10);
	//Mesh tableMesh = Mesh("table.obj", 5);

	// Construct the Cornell Box
	transform = glm::translate(glm::mat4(1.0f), { 0,-0.025,0 });
	Box floor = Box(transform, &whiteMat, { 1.5,0.05,1.5 });
	scene.AddObject(&floor);

	transform = glm::translate(glm::mat4(1.0f), { 0, 1.325, 0 });
	Box ceiling = Box(transform, &whiteMat, { 2, 0.05, 2 });
	scene.AddObject(&ceiling);

	transform = glm::translate(glm::mat4(1.0f), { -0.75025,1,0 });
	Box leftWall = Box(transform, &redWallMat, { 0.05, 2, 2 });
	scene.AddObject(&leftWall);

	transform = glm::translate(glm::mat4(1.0f), { 0.75025,1,0 });
	Box rightWall = Box(transform, &greenWallMat, { 0.05,2,2 });
	scene.AddObject(&rightWall);

	transform = glm::translate(glm::mat4(1.0f), { 0,1,-0.75025 });
	Box backWall = Box(transform, &whiteMat, { 2,2,0.05 });
	scene.AddObject(&backWall);

	/*transform = glm::translate(glm::mat4(1.0f), { -0.3, 0.4, -0.3 });
	transform = glm::rotate(transform, glm::radians(20.0f), { 0,1,0 });
	Box leftBox = Box(transform, &whiteMat, { 0.4, 0.8, 0.4 });
	scene.AddObject(&leftBox);*/

	transform = glm::translate(glm::mat4(1.0f), { -0.3, 0.3, -0.3 });
	transform = glm::scale(transform, { 0.3,0.3,0.3 });
	Sphere leftSphere = Sphere(transform, &whiteMat);
	scene.AddObject(&leftSphere);

	transform = glm::translate(glm::mat4(1.0f), { 0.25, 0.2, 0.2 });
	transform = glm::rotate(transform, glm::radians(-20.0f), { 0,1,0 });
	Box rightBox = Box(transform, &whiteMat, { 0.4, 0.4, 0.4 });
	scene.AddObject(&rightBox);

	transform = glm::translate(glm::mat4(1.0f), { 0.25,0.4,0.2 });
	transform = glm::rotate(transform, glm::radians(-90.0f), { 1,0,0 });
	transform = glm::scale(transform, { 0.025/3.0, 0.025/3.0, 0.025/3.0 });
	MeshInstance teapot = MeshInstance(&teapotMesh, transform, &whiteShinyMat);
	//scene.AddObject(&teapot);


	// Render and save the image. The image saves in the TestProgram folder (when I run the project that's where VS puts it at least)

	time_t startTime = time(0);
	scene.RenderScene();
	time_t endTime = time(0);

	time_t deltaTime = endTime - startTime;

	scene.SaveScene("Output\\image");

	float hours = deltaTime / (60.0f * 60.0f);
	int wholeHours = int(hours);

	float minutes = (hours - wholeHours) * 60.0f;
	int wholeMinutes = int(minutes);

	float seconds = (minutes - wholeMinutes) * 60.0f;
	int wholeSeconds = int(seconds);


	std::cout << "Time to render: " << wholeHours << " hours, " << wholeMinutes << " minutes, " << wholeSeconds << " seconds." << std::endl;
	system("PAUSE");
}