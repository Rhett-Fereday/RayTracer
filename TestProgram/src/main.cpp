#include "Scene.h"
#include "Camera.h"
#include "Sphere.h"
#include "Box.h"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>
#include "ConstMaterial.h"
#include "DirectionalLight.h"
#include "PointLight.h"
#include "SpotLight.h"

int main(int argc, char* argv[])
{
	RayTracer::Camera camera = RayTracer::Camera(1920,1080);
	RayTracer::Scene scene(camera);

	RayTracer::DirectionalLight sunlight = RayTracer::DirectionalLight({ -1,-1,-1 }, { 1,1,1 }, 1.0f);
	RayTracer::PointLight pointLight = RayTracer::PointLight({ 1,1,1 }, 10.0f, { -0.5f,0,-4 });
	RayTracer::SpotLight spotLight = RayTracer::SpotLight({ 1,1,1 }, 10.0f, { 0,0,-1 }, { 0,0,-1 }, 15.0f);

	//scene.AddLight(&sunlight);
	//scene.AddLight(&pointLight);
	scene.AddLight(&spotLight);


	glm::mat4 transform = glm::translate(glm::mat4(1.0f), glm::vec3(-1.0f, 0, -4));
	transform = glm::rotate(transform, glm::radians(45.0f), glm::vec3(1, 0, 0));

	RayTracer::ConstMaterial redMat, blueMat, greenMat, whiteMat;
	redMat.albedo = { 1,0,0 }; redMat.reflectiveness = 0.8f;
	blueMat.albedo = { 0,0,1 }; blueMat.reflectiveness = 0.7f;
	greenMat.albedo = { 0,1,0 }; greenMat.reflectiveness = 0.0f;
	whiteMat.albedo = { 1,1,1 }; whiteMat.reflectiveness = 0.0f;

	RayTracer::Sphere leftSphere = RayTracer::Sphere(transform, &redMat, 0.3f);

	transform = glm::translate(glm::mat4(1.0f), glm::vec3(1.5f, 0, -5));
	transform = glm::rotate(transform, glm::radians(-45.0f), glm::vec3(0, 1, 0));
	RayTracer::Sphere rightSphere = RayTracer::Sphere(transform, &blueMat, 0.5f);
	
	transform = glm::translate(glm::mat4(1.0f), glm::vec3(0, 0.5, -5));
	transform = glm::rotate(transform, glm::radians(30.0f), glm::vec3(0, 1, 0));

	//RayTracer::Sphere midSphere = RayTracer::Sphere(transform, &greenMat, 0.5f);
	
	scene.AddObject(&leftSphere);	
	scene.AddObject(&rightSphere);

	// Create Room
	transform = glm::translate(glm::mat4(1.0f), glm::vec3(0, -0.55, -5));
	RayTracer::Box floor = RayTracer::Box(transform, &whiteMat, glm::vec3(-1.5, -0.05, -1.5), glm::vec3(1.5, 0.05, 1.5));

	transform = glm::translate(glm::mat4(1.0f), glm::vec3(0, 0.2, -5));
	transform = glm::rotate(transform, glm::radians(90.0f), glm::vec3(1, 0, 0));
	RayTracer::Box backWall = RayTracer::Box(transform, &greenMat, glm::vec3(-1.5, -0.05, -0.75), glm::vec3(1.5, 0.05, 0.75));

	transform = glm::translate(glm::mat4(1.0f), glm::vec3(-1.5, 0.2, -5));
	transform = glm::rotate(transform, glm::radians(90.0f), glm::vec3(0, 0, 1));
	RayTracer::Box leftWall = RayTracer::Box(transform, &blueMat, glm::vec3(-0.75, -0.05, -1.5), glm::vec3(0.75, 0.05, 1.5));

	transform = glm::translate(glm::mat4(1.0f), glm::vec3(0.5, 0.2, -5));
	transform = glm::rotate(transform, glm::radians(90.0f), glm::vec3(0, 0, 1));
	RayTracer::Box rightWall = RayTracer::Box(transform, &redMat, glm::vec3(-0.75, -0.05, -1.5), glm::vec3(0.75, 0.05, 1.5));

	scene.AddObject(&floor);
	scene.AddObject(&backWall);
	scene.AddObject(&leftWall);
	scene.AddObject(&rightWall);

	scene.RenderScene();
	scene.SaveScene("TestImage.ppm");
}