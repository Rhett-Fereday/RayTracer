#pragma once

#include "glm/glm.hpp"
#include "CImg.h"
#include "Core.h"

namespace RayTracer
{
	class RT_API Camera
	{
	friend class Scene;

	public:
		Camera(unsigned int width = 1920, unsigned int height = 1080, float fov = 45, glm::vec3 position = { -5,2,0 }, glm::vec3 lookAt = { 0,0,0 }, glm::vec3 upVector = { 0,1,0 });

	protected:
		void Render();
		cimg_library::CImg<unsigned char> GetRenderTarget();

	private:
		unsigned int m_width, m_height;
		float m_fov;
		glm::vec3 m_position, m_lookAt, m_upVector;
		cimg_library::CImg<unsigned char> m_renderTarget;
	};
}