#include "Camera.h"
#include "Scene.h"
#include "glm/glm.hpp"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>

namespace RayTracer
{
	Camera::Camera(unsigned int width, unsigned int height, float fov, glm::vec3 position, glm::vec3 lookAt, glm::vec3 upVector) : m_width(width), m_height(height), m_fov(fov), m_position(position)
	{
		m_renderTarget.assign(m_width, m_height, 1, 3);

        glm::vec3 center = lookAt - position;
        m_worldToCamera = glm::lookAt(position, lookAt, upVector);
        m_cameraToWorld = glm::inverse(m_worldToCamera);
	}
	
	void Camera::Render(Scene* scene)
	{
        float widthInverse = 1 / float(m_width);
        float heightInverse = 1 / float(m_height);
        float fieldOfView = m_fov;
        float aspectRatio = float(m_width) / float(m_height);
        float angle = tan(3.14 * 0.5f * fieldOfView / 180.0f);

        for (int y = 0; y < m_height; y++)
        {
            for (int x = 0; x < m_width; x++)
            {
                float i = (2 * ((x + 0.5f) * widthInverse) - 1) * angle * aspectRatio;
                float j = (1 - 2 * ((y + 0.5f) * heightInverse)) * angle;
                glm::vec3 ray(i, j, -1);
                glm::vec4 temp = glm::normalize(m_cameraToWorld * glm::vec4(ray, 1));
                ray = glm::normalize(glm::vec3(temp.x, temp.y, temp.z));
                glm::vec4 rayOrigin = m_cameraToWorld * glm::vec4(0);

                glm::vec3 color = scene->TraceRay({ rayOrigin.x, rayOrigin.y, rayOrigin.z }, ray, glm::vec3(1.0f), 1);

                /*if (color.r > 1 || color.g > 1 || color.b > 1)
                {
                    float max = std::max(color.r, std::max(color.g, color.b));
                    color.r = color.r / max;
                    color.g = color.g / max;
                    color.b = color.b / max;
                }*/
                glm::vec3 clampedColor = glm::clamp(color, { 0,0,0 }, { 1,1,1 });
                m_renderTarget(x, y, 0, 0) = (unsigned char)(clampedColor.r * 255);
                m_renderTarget(x, y, 0, 1) = (unsigned char)(clampedColor.g * 255);
                m_renderTarget(x, y, 0, 2) = (unsigned char)(clampedColor.b * 255);
            }
        }
	}
	
	cimg_library::CImg<unsigned char> Camera::GetRenderTarget()
	{
		return m_renderTarget;
	}
}