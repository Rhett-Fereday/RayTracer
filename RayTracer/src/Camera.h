#pragma once

#define GLM_FORCE_INTRINSICS
#define GLM_FORCE_MESSAGES
#include "glm/glm.hpp"
#include "CImg.h"
#include "Core.h"
#include "PostProcesses/PostProcess.h"
#include <mutex>
#include <queue>
#include <vector>

namespace RayTracer
{
	class RT_API Camera
	{
	friend class Scene;

	public:
		Camera(unsigned int width = 1920, unsigned int height = 1080, float fov = 90, glm::vec3 position = { 0,0,0 }, glm::vec3 lookAt = { 0.0f, 0.0f, -1.0f }, float lensRadius = 0.045f, glm::vec3 upVector = { 0,1,0 });
		void AddPostProcess(PostProcess* process);

	protected:
		void Render(Scene* scene);
		cimg_library::CImg<unsigned char> GetRenderTarget();

	private:
		unsigned int m_width, m_height, m_raysPerPixel, m_numberOfThreads;
		float m_fov, m_focalDistance, m_lensRadius;
		glm::vec3 m_position;
		glm::mat4 m_worldToCamera, m_cameraToWorld;
		glm::vec3** m_hdrBuffer;
		cimg_library::CImg<unsigned char> m_renderTarget;
		std::mutex m_renderMutex;
		std::queue<std::pair<int, int>> m_renderQueue;
		std::vector<PostProcess*> m_postProcesses;

		void ThreadRender(Scene* scene);
	};
}