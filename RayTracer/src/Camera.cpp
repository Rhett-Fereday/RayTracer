#include "Camera.h"
#include "Scene.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtx/transform.hpp"

#include <cstdlib>
#include <ctime>

namespace RayTracer
{
	Camera::Camera(unsigned int width, unsigned int height, float fov, glm::vec3 position, glm::vec3 lookAt, glm::vec3 upVector) : m_width(width), m_height(height), m_fov(fov), m_position(position)
	{
		m_renderTarget.assign(m_width, m_height, 1, 3);

        glm::vec3 center = lookAt - position;
        m_worldToCamera = glm::lookAt(position, lookAt, upVector);
        m_cameraToWorld = glm::inverse(m_worldToCamera);
		m_raysPerPixel = 1;
	}
	
    // Cast the camera rays into the scene. Code set up from scratchapixel.com with modifications to support camera transforms
	void Camera::Render(Scene* scene)
	{
        float widthInverse = 1 / float(m_width);
        float heightInverse = 1 / float(m_height);
        float fieldOfView = m_fov;
        float aspectRatio = float(m_width) / float(m_height);
        float angle = tan(3.14 * 0.5f * fieldOfView / 180.0f);
		float xOffset, yOffset;		

		srand(static_cast <unsigned> (time(0)));

		glm::vec4 rayOrigin = m_cameraToWorld * glm::vec4(0,0,0,1);

        for (int y = 0; y < m_height; y++)
        {
            for (int x = 0; x < m_width; x++)
            {
				glm::vec3 color = { 0,0,0 };

				for (int r = 0; r < m_raysPerPixel; r++)
				{
					xOffset = 0;// -0.5f + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (0.5f - -0.5f)));
					yOffset = 0;// -0.5f + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (0.5f - -0.5f)));
					float i = (2 * ((x + 0.5f + xOffset) * widthInverse) - 1) * angle * aspectRatio;
					float j = (1 - 2 * ((y + 0.5f + yOffset) * heightInverse)) * angle;
					glm::vec3 ray(i, j, -1);

					// Transform the rays from camera space into world space.
					glm::vec4 temp = glm::normalize(m_cameraToWorld * glm::vec4(ray, 0));
					ray = glm::normalize(glm::vec3(temp.x, temp.y, temp.z));

					// Cast the ray using the Scene class' recursive TraceRay method
					color += scene->TraceRay({ rayOrigin.x, rayOrigin.y, rayOrigin.z }, ray, glm::vec3(1.0f), 1);
				}

				color = color / float(m_raysPerPixel);

                // Clamp returned value just to be safe
                glm::vec3 clampedColor = glm::clamp(color, { 0,0,0 }, { 1,1,1 });

                // Store in the cimg object
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