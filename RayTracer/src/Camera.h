#pragma once

#include "glm/glm.hpp"
#include "CImg.h"

namespace RayTracer
{
	class Camera
	{
	public:
		Camera(unsigned int width = 1920, unsigned int height = 1080, float fov = 45, glm::vec3 position = { -5,2,0 }, glm::vec3 lookAt = { 0,0,0 }, glm::vec3 upVector = { 0,1,0 });

	private:
		unsigned int m_width, m_height;
		float m_fov;
		glm::vec3 m_position, m_lookAt, m_upVector;
		cimg_library::CImg<unsigned char> m_renderTarget;
	};
}