#include "Camera.h"

namespace RayTracer
{
	Camera::Camera(unsigned int width, unsigned int height, float fov, glm::vec3 position, glm::vec3 lookAt, glm::vec3 upVector)
		: m_width(width), m_height(height), m_fov(fov), m_position(position), m_lookAt(lookAt), m_upVector(upVector)
	{
		m_renderTarget.assign(m_width, m_height, 1, 3);
	}
	
	void Camera::Render()
	{
        for (int y = 0; y < m_height; y++)
        {
            for (int x = 0; x < m_width; x++)
            {
                /*float i = (2 * ((x + 0.5f) * widthInverse) - 1) * angle * aspectRatio;
                float j = (1 - 2 * ((y + 0.5f) * heightInverse)) * angle;
                vec3 ray(i, j, -1);
                normalize(ray);
                vec3 color = TraceRay(vec3(0), ray, vec3(1.0f), 0, objects, lights);

                if (color.r > 1 || color.g > 1 || color.b > 1)
                {
                    float max = std::max(color.r, std::max(color.g, color.b));
                    color.r = color.r / max;
                    color.g = color.g / max;
                    color.b = color.b / max;
                }*/

                m_renderTarget(x, y, 0, 0) = (unsigned char)(255);
                m_renderTarget(x, y, 0, 1) = (unsigned char)(128);
                m_renderTarget(x, y, 0, 2) = (unsigned char)(128);
            }
        }
	}
	
	cimg_library::CImg<unsigned char> Camera::GetRenderTarget()
	{
		return m_renderTarget;
	}
}