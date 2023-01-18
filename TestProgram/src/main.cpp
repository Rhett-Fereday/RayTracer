#include "Scene.h"
#include "Camera/Camera.h"
#include "Camera/Lenses/PinholeLens.h"
#include "Camera/Lenses/ThinLens.h"
#include "Integrators/RayCaster.h"
#include "Integrators/PathTracer.h"
#include "SampleGenerators/RandomGenerator.h"
#include "SampleGenerators/StratifiedGenerator.h"

#include "Materials/Material.h"
#include "Materials/ConstantTexture.h"
#include "Renderables/Renderable.h"
#include "Renderables/Geometry/Sphere.h"
#include "Renderables/Geometry/Plane.h"
#include "Renderables/Geometry/Disk.h"
#include "Renderables/Geometry/Box.h"
#include "Renderables/Lights/Light.h"
#include "Renderables/Lights/AreaLight.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>

#include <time.h>
#include <iostream>

using namespace RayTracer;

enum RenderScene
{
	CORNELL_BOX,
	MANY_LIGHT_CORNELL,
	PARKING_GARAGE
};

enum CameraLens
{
	PINHOLE_LENS,
	THIN_LENS
};

enum IntegratorType
{
	RAY_CASTER,
	PATH_TRACER
};

enum GeneratorType
{
	RANDOM_GENERATOR,
	STRATIFIED_GENERATOR
};

void RenderCornellBox(shared_ptr<Lens> lens, IntegratorType integratorType, 
	GeneratorType generatorType, unsigned int samplesPerPixel, 
	unsigned int imageWidth, unsigned int imageHeight, int numThreads)
{
	auto scene = make_shared<Scene>();

	// Create the materials needed
	auto redMat = make_shared<Material>(make_shared<ConstantTexture>(vec3(0.545f, 0.0f, 0.0f)));
	auto whiteMat = make_shared<Material>(make_shared<ConstantTexture>(vec3(1)));
	auto greenMat = make_shared<Material>(make_shared<ConstantTexture>(vec3(86.0f / 255.0f, 125.0f / 255.0f, 70.0f / 255.0f)));

	// Add the floor to the scene
	mat4 transform = glm::translate(vec3(0, 0, 0));
	transform = glm::rotate(transform, glm::radians(-90.0f), vec3(1, 0, 0));
	auto floorGeom = make_shared<Plane>(transform, 4.0f);
	auto floor = make_shared<Renderable>(floorGeom, whiteMat);
	scene->AddRenderable(floor);

	// Add the back wall to the scene
	transform = glm::translate(vec3(0, 0.3, -0.75));
	auto backWallGeom = make_shared<Plane>(transform, 2.0f);
	auto backWall = make_shared<Renderable>(backWallGeom, whiteMat);
	scene->AddRenderable(backWall);

	// Add the front wall (behind camera)
	transform = glm::translate(vec3(0, 0.3, 2));
	transform = glm::rotate(transform, glm::radians(180.0f), vec3(0, 1, 0));
	auto frontWallGeom = make_shared<Plane>(transform, 2.0f);
	auto frontWall = make_shared<Renderable>(frontWallGeom, whiteMat);
	scene->AddRenderable(frontWall);

	// Add left wall to the scene
	transform = glm::translate(vec3(-0.75, -0.7, 0));
	transform = glm::rotate(transform, glm::radians(90.0f), vec3(0, 1, 0));
	auto leftWallGeom = make_shared<Plane>(transform, 4.0f);
	auto leftWall = make_shared<Renderable>(leftWallGeom, redMat);
	scene->AddRenderable(leftWall);

	// Add right wall to the scene
	transform = glm::translate(vec3(0.75, -0.7, 0));
	transform = glm::rotate(transform, glm::radians(-90.0f), vec3(0, 1, 0));
	auto rightWallGeom = make_shared<Plane>(transform, 4.0f);
	auto rightWall = make_shared<Renderable>(rightWallGeom, greenMat);
	scene->AddRenderable(rightWall);

	// Add the ceiling to the scene
	transform = glm::translate(vec3(0, 1.3, 0));
	transform = glm::rotate(transform, glm::radians(90.0f), vec3(1,0,0));
	auto ceilingGeom = make_shared<Plane>(transform, 2.0f);
	auto ceiling = make_shared<Renderable>(ceilingGeom, whiteMat);
	scene->AddRenderable(ceiling);

	// Add a sphere to the scene
	/*transform = glm::translate(vec3(-0.3, 0.3, -0.3));
	transform = glm::rotate(transform, glm::radians(-45.0f), vec3(0, 1, 0));
	auto sphereGeom = make_shared<Sphere>(0.3f, transform);
	auto sphere = make_shared<Renderable>(sphereGeom, whiteMat);
	scene->AddRenderable(sphere);*/

	// Add the left box to the scene
	transform = glm::translate(glm::mat4(1.0f), { -0.3, 0.4, -0.3 });
	transform = glm::rotate(transform, glm::radians(20.0f), { 0,1,0 });
	auto boxGeom = make_shared<Box>(transform, vec3(0.4, 0.8, 0.4));
	auto leftBox = make_shared<Renderable>(boxGeom, whiteMat);
	scene->AddRenderable(leftBox);

	// Add the right box to the scene
	transform = glm::translate(glm::mat4(1.0f), { 0.25, 0.2, 0.2 });
	transform = glm::rotate(transform, glm::radians(-20.0f), { 0,1,0 });
	boxGeom = make_shared<Box>(transform, vec3(0.4, 0.4, 0.4));
	auto rightBox = make_shared<Renderable>(boxGeom, whiteMat);
	scene->AddRenderable(rightBox);

	// Add the area light to the scene
	transform = glm::translate(vec3(0, 1.299f, 0));
	transform = glm::rotate(transform, glm::radians(90.0f), vec3(1,0,0));
	//transform = glm::translate(vec3(0, 1.199f, 0));
	auto lightGeom = make_shared<Plane>(transform, .25f); //make_shared<Sphere>(0.1, transform);
	auto light = make_shared<AreaLight>(transform, lightGeom, vec3(1, 214.0f / 255.0f, 170.0f / 255.0f), 60.f, 1);
	scene->AddLight(light);

	CameraSettings settings;
	settings.samplesPerPixel = samplesPerPixel;
	settings.imageWidth = imageWidth;
	settings.imageHeight = imageHeight;
	settings.position = { 0, 0.65, 1.99 };
	settings.lookAt = { 0, 0.65, 0 };
	settings.fov = 45.0f;

	auto camera = make_shared<Camera>(lens, settings);

	shared_ptr<Integrator> integrator;
	switch (integratorType)
	{
	case IntegratorType::PATH_TRACER:
		integrator = make_shared<PathTracer>(4);
		break;

	default:
		integrator = make_shared<RayCaster>();
		break;
	}

	shared_ptr<SampleGenerator> generator;
	switch (generatorType)
	{
	default:
		generator = make_shared<StratifiedGenerator>();
		break;
	}	

	integrator->Render(scene, camera, generator, numThreads);

	camera->SaveImage("CornellBox");
}

void RenderManyLightCornell(shared_ptr<Lens> lens, IntegratorType integratorType,
	GeneratorType generatorType, unsigned int samplesPerPixel,
	unsigned int imageWidth, unsigned int imageHeight, int numThreads)
{
	auto scene = make_shared<Scene>();

	// Create the materials needed
	auto redMat = make_shared<Material>(make_shared<ConstantTexture>(vec3(0.545f, 0.0f, 0.0f)));
	auto whiteMat = make_shared<Material>(make_shared<ConstantTexture>(vec3(1)));
	auto greenMat = make_shared<Material>(make_shared<ConstantTexture>(vec3(86.0f / 255.0f, 125.0f / 255.0f, 70.0f / 255.0f)));

	// Add the floor to the scene
	mat4 transform = glm::translate(vec3(0, 0, 0));
	transform = glm::rotate(transform, glm::radians(-90.0f), vec3(1, 0, 0));
	auto floorGeom = make_shared<Plane>(transform, 4.0f);
	auto floor = make_shared<Renderable>(floorGeom, whiteMat);
	scene->AddRenderable(floor);

	// Add the back wall to the scene
	transform = glm::translate(vec3(0, 0.3, -0.75));
	auto backWallGeom = make_shared<Plane>(transform, 2.0f);
	auto backWall = make_shared<Renderable>(backWallGeom, whiteMat);
	scene->AddRenderable(backWall);

	// Add the front wall (behind camera)
	transform = glm::translate(vec3(0, 0.3, 2));
	transform = glm::rotate(transform, glm::radians(180.0f), vec3(0, 1, 0));
	auto frontWallGeom = make_shared<Plane>(transform, 2.0f);
	auto frontWall = make_shared<Renderable>(frontWallGeom, whiteMat);
	scene->AddRenderable(frontWall);

	// Add left wall to the scene
	transform = glm::translate(vec3(-0.75, -0.7, 0));
	transform = glm::rotate(transform, glm::radians(90.0f), vec3(0, 1, 0));
	auto leftWallGeom = make_shared<Plane>(transform, 4.0f);
	auto leftWall = make_shared<Renderable>(leftWallGeom, redMat);
	scene->AddRenderable(leftWall);

	// Add right wall to the scene
	transform = glm::translate(vec3(0.75, -0.7, 0));
	transform = glm::rotate(transform, glm::radians(-90.0f), vec3(0, 1, 0));
	auto rightWallGeom = make_shared<Plane>(transform, 4.0f);
	auto rightWall = make_shared<Renderable>(rightWallGeom, greenMat);
	scene->AddRenderable(rightWall);

	// Add the ceiling to the scene
	transform = glm::translate(vec3(0, 1.3, 0));
	transform = glm::rotate(transform, glm::radians(90.0f), vec3(1, 0, 0));
	auto ceilingGeom = make_shared<Plane>(transform, 2.0f);
	auto ceiling = make_shared<Renderable>(ceilingGeom, whiteMat);
	scene->AddRenderable(ceiling);

	// Add the left box to the scene
	transform = glm::translate(glm::mat4(1.0f), { -0.3, 0.4, -0.3 });
	transform = glm::rotate(transform, glm::radians(20.0f), { 0,1,0 });
	auto boxGeom = make_shared<Box>(transform, vec3(0.4, 0.8, 0.4));
	auto leftBox = make_shared<Renderable>(boxGeom, whiteMat);
	scene->AddRenderable(leftBox);

	// Add the right box to the scene
	transform = glm::translate(glm::mat4(1.0f), { 0.25, 0.2, 0.2 });
	transform = glm::rotate(transform, glm::radians(-20.0f), { 0,1,0 });
	boxGeom = make_shared<Box>(transform, vec3(0.4, 0.4, 0.4));
	auto rightBox = make_shared<Renderable>(boxGeom, whiteMat);
	scene->AddRenderable(rightBox);

	CameraSettings settings;
	settings.samplesPerPixel = samplesPerPixel;
	settings.imageWidth = imageWidth;
	settings.imageHeight = imageHeight;
	settings.position = { 0, 0.65, 1.99 };
	settings.lookAt = { 0, 0.65, 0 };
	settings.fov = 45.0f;

	auto camera = make_shared<Camera>(lens, settings);

	shared_ptr<Integrator> integrator;
	switch (integratorType)
	{
	case IntegratorType::PATH_TRACER:
		integrator = make_shared<PathTracer>(4);
		break;

	default:
		integrator = make_shared<RayCaster>();
		break;
	}

	shared_ptr<SampleGenerator> generator;
	switch (generatorType)
	{
	default:
		generator = make_shared<StratifiedGenerator>();
		break;
	}

	// Add the lights to the scene
	int lightsX = 6;
	int lightsZ = 6;
	double minRadius = 0.02;
	double maxRadius = 0.02;	
	
	double gridStepX = 1.5 / static_cast<double>(lightsX);
	double gridStepY = 1.5 / static_cast<double>(lightsZ);

	double maxLuminance = 15.0;
	double minLuminance = 12.0;
	double minY = 1.25;
	double maxY = 1.27;

	for (int i = 0; i < lightsX; i++)
	{
		for (int j = 0; j < lightsZ; j++)
		{
			double radius = minRadius + generator->Generate1D() * (maxRadius - minRadius);
			double luminance = minLuminance + generator->Generate1D() * (maxLuminance - minLuminance);
			double x = static_cast<double>(i) * gridStepX + gridStepX/2.0 - 0.75;
			double y = minY + generator->Generate1D() * (maxY - minY);
			double z = static_cast<double>(j) * gridStepY + gridStepY/2.0 - 0.75;

			transform = glm::translate(vec3(x, y, z));
			auto lightGeom = make_shared<Sphere>(radius, transform);
			auto light = make_shared<AreaLight>(transform, lightGeom, vec3(1, 214.0f / 255.0f, 170.0f / 255.0f), luminance, 1);
			scene->AddLight(light);
		}
	}

	integrator->Render(scene, camera, generator, numThreads);

	camera->SaveImage("ManyLightCornell");
}

void RenderParkingGarage(shared_ptr<Lens> lens, IntegratorType integratorType,
	GeneratorType generatorType, unsigned int samplesPerPixel,
	unsigned int imageWidth, unsigned int imageHeight, int numThreads)
{
	auto scene = make_shared<Scene>(vec3(1));

	// Create the materials needed
	auto whiteMat = make_shared<Material>(make_shared<ConstantTexture>(vec3(1)));
	auto redMat = make_shared<Material>(make_shared<ConstantTexture>(vec3(1,0,0)));

	// Add the floor to the scene
	mat4 transform = glm::translate(vec3(0, 0, -25.0f));
	transform = glm::rotate(transform, glm::radians(-90.0f), vec3(1, 0, 0));
	auto floorGeom = make_shared<Plane>(transform, 50.0f);
	auto floor = make_shared<Renderable>(floorGeom, whiteMat);
	scene->AddRenderable(floor);

	// Add the back wall to the scene
	transform = glm::translate(vec3(0, 1.5f, -50.0f));
	auto backWallGeom = make_shared<Plane>(transform, 3.0f, 50.0f);
	auto backWall = make_shared<Renderable>(backWallGeom, redMat);
	scene->AddRenderable(backWall);

	// Add the ceiling to the scene
	transform = glm::translate(vec3(0, 3.0f, -25.0f));
	transform = glm::rotate(transform, glm::radians(90.0f), vec3(1, 0, 0));
	auto ceilingGeom = make_shared<Plane>(transform, 50.0f);
	auto ceiling = make_shared<Renderable>(ceilingGeom, whiteMat);
	scene->AddRenderable(ceiling);

	CameraSettings settings;
	settings.samplesPerPixel = samplesPerPixel;
	settings.imageWidth = imageWidth;
	settings.imageHeight = imageHeight;
	settings.position = { 0, 1.5f, 0 };
	settings.lookAt = { 0, 1.5f, -1 };
	settings.fov = 90.0f;

	auto camera = make_shared<Camera>(lens, settings);

	shared_ptr<Integrator> integrator;
	switch (integratorType)
	{
	case IntegratorType::PATH_TRACER:
		integrator = make_shared<PathTracer>(4);
		break;

	default:
		integrator = make_shared<RayCaster>();
		break;
	}

	shared_ptr<SampleGenerator> generator;
	switch (generatorType)
	{
	default:
		generator = make_shared<StratifiedGenerator>();
		break;
	}

	integrator->Render(scene, camera, generator, numThreads);

	camera->SaveImage("ParkingGarage");
}

int main(int argc, char* argv[])
{
	auto sceneToRender = RenderScene::MANY_LIGHT_CORNELL;
	auto lensToUse = CameraLens::PINHOLE_LENS;
	auto integratorToUse = IntegratorType::PATH_TRACER;
	auto generatorToUse = GeneratorType::STRATIFIED_GENERATOR;
	unsigned int numThreads = std::thread::hardware_concurrency() - 1;
	unsigned int samplesPerPixel = 36;
	unsigned int imageWidth = 640;
	unsigned int imageHeight = 480;

	shared_ptr<Lens> lens;

	switch (lensToUse)
	{
	case CameraLens::THIN_LENS:
		lens = make_shared<ThinLens>(0.01f);
		break;

	default:
		lens = make_shared<PinholeLens>();
	}

	time_t startTime = time(0);

	switch (sceneToRender)
	{
	case RenderScene::MANY_LIGHT_CORNELL:
		RenderManyLightCornell(lens, integratorToUse, generatorToUse, samplesPerPixel, imageWidth, imageHeight, numThreads);
		break;

	case RenderScene::PARKING_GARAGE:
		RenderParkingGarage(lens, integratorToUse, generatorToUse, samplesPerPixel, imageWidth, imageHeight, numThreads);
		break;

	default:
		RenderCornellBox(lens, integratorToUse, generatorToUse, samplesPerPixel, imageWidth, imageHeight, numThreads);
		break;
	}

	time_t endTime = time(0);

	time_t deltaTime = endTime - startTime;

	float hours = deltaTime / (60.0f * 60.0f);
	int wholeHours = int(hours);

	float minutes = (hours - wholeHours) * 60.0f;
	int wholeMinutes = int(minutes);

	float seconds = (minutes - wholeMinutes) * 60.0f;
	int wholeSeconds = int(seconds);

	std::cout << "Time to render: " << wholeHours << " hours, " << wholeMinutes << " minutes, " << wholeSeconds << " seconds." << std::endl;
	system("PAUSE");
}