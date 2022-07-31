#pragma once

#define GLM_FORCE_INTRINSICS
#define GLM_FORCE_MESSAGES
#include "glm/glm.hpp"
#include "CImg.h"
#include "../Core.h"
#include "../Ray.h"
#include "Lenses/Lens.h"
#include "PostProcesses/PostProcessGroup.h"
#include <string>
#include <mutex>
#include <queue>
#include <vector>
#include <memory>

namespace RayTracer
{
	struct RT_API CameraSettings
	{
		unsigned int samplesPerPixel = 4;
		unsigned int imageWidth = 500;
		unsigned int imageHeight = 500;
		float fov = 90;
		float focalDistance = 1.0f;
		vec3 position = { 0,0,0 };
		vec3 lookAt = { 0,0,-1 };
		vec3 upVector = { 0,1,0 };
	};

	class RT_API Camera
	{
		friend class Integrator;

	public:
		Camera(std::shared_ptr<Lens> lens, const CameraSettings& cameraSettings);

		Ray GenerateRay(int x, int y, vec2 uv1, vec2 uv2) const;

		unsigned int ImageHeight() const;
		unsigned int ImageWidth() const;
		unsigned int SamplesPerPixel() const;

		void SaveImage(std::string imageName);

	private:
		const CameraSettings settings;
		const float widthInverse, heightInverse, aspectRatio, angle;
		glm::mat4 worldToCamera, cameraToWorld;
		std::shared_ptr<Lens> lens;
		std::vector<std::vector<vec3>> imageBuffer;
		shared_ptr<PostProcessGroup> processGroup;
	};

	/*class RT_API Camera
	{
	friend class Scene;

	public:
		Camera(unsigned int width = 1920, unsigned int height = 1080, float fov = 90, glm::vec3 position = { 0,0,0 }, glm::vec3 lookAt = { 0.0f, 0.0f, -1.0f }, float lensRadius = 0.045f, glm::vec3 upVector = { 0,1,0 });
		void AddPostProcessGroup(PostProcessGroup* processGroup);
		void SaveRender(const char* filename);

	protected:
		void Render(Scene* scene);
		cimg_library::CImg<unsigned char> GetRenderTarget();

	private:
		unsigned int m_width, m_height, m_raysPerPixel, m_numberOfThreads;
		float m_fov, m_focalDistance, m_lensRadius;
		glm::vec3 m_position;
		glm::mat4 m_worldToCamera, m_cameraToWorld;
		glm::vec3** m_rawHDRBuffer;
		glm::vec3** m_postProcessBuffer;
		cimg_library::CImg<unsigned char> m_renderTarget;
		std::mutex m_renderMutex;
		std::queue<std::pair<int, int>> m_renderQueue;
		std::vector<PostProcessGroup*> m_postProcessGroups;

		void ThreadRender(Scene* scene);
	}; */
}