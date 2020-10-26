#include "Camera.h"
#include "Scene.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtx/transform.hpp"

#include <thread>
#include <cstdlib>
#include <ctime>

namespace RayTracer
{
	Camera::Camera(unsigned int width, unsigned int height, float fov, glm::vec3 position, glm::vec3 lookAt, float lensRadius, glm::vec3 upVector) : m_width(width), m_height(height), m_fov(fov), m_position(position), m_lensRadius(lensRadius)
	{
		m_renderTarget.assign(m_width, m_height, 1, 3);

        m_worldToCamera = glm::lookAt(position, lookAt, upVector);
        m_cameraToWorld = glm::inverse(m_worldToCamera);
		m_raysPerPixel = 1024;
		m_numberOfThreads = std::thread::hardware_concurrency();

		m_focalDistance = glm::distance(position, lookAt); // Set focus plane to the point we want to look at
	}
	
    // Cast the camera rays into the scene. Code set up from scratchapixel.com with modifications to support camera transforms
	void Camera::Render(Scene* scene)
	{
		srand(static_cast <unsigned> (time(0)));
		std::thread *threads = new std::thread[m_numberOfThreads];

		for (int i = 0; i < m_numberOfThreads - 1; i++)
		{
			threads[i] = std::thread(&Camera::ThreadRender, this, scene, i);
		}

		ThreadRender(scene, m_numberOfThreads - 1);

		for (int t = 0; t < m_numberOfThreads - 1; t++)
		{
			threads[t].join();
		}
	}
	
	cimg_library::CImg<unsigned char> Camera::GetRenderTarget()
	{
		return m_renderTarget;
	}

	void Camera::ThreadRender(Scene * scene, int threadID)
	{
		int maxConcurrentThreads = m_numberOfThreads;
		int numTilesWide = sqrt(maxConcurrentThreads);
		int numTilesHigh = numTilesWide;

		// If the maxConcurrentThreads isn't a perfect square we must handle it
		if ((maxConcurrentThreads - numTilesWide * numTilesWide) > 0) numTilesHigh = maxConcurrentThreads / numTilesWide;

		int tilePixelWidth = m_width / numTilesWide;
		int tilePixelHeight = m_height / numTilesHigh;

		int tilePixelWidthStartIndex = (threadID % numTilesWide)  * tilePixelWidth;
		int tilePixelWidthEndIndex = (threadID % numTilesWide) * tilePixelWidth + tilePixelWidth;
		int tilePixelHeightStartIndex = (int)(threadID / numTilesWide) * tilePixelHeight;
		int tilePixelHeightEndIndex = (int)(threadID / numTilesWide) * tilePixelHeight + tilePixelHeight;

		float fwidth = m_width;
		float widthInverse = 1.0f / fwidth;
		float heightInverse = 1.0f / (float)m_height;
		float fieldOfView = m_fov;
		float aspectRatio = (float)m_width / (float)m_height;
		float angle = tan(3.14 * 0.5f * fieldOfView / 180.0f);

		for (int y = tilePixelHeightStartIndex; y < tilePixelHeightEndIndex; y++)
		{
			for (int x = tilePixelWidthStartIndex; x < tilePixelWidthEndIndex; x++)
			{
				glm::vec3 color = { 0,0,0 };

				for (int r = 0; r < m_raysPerPixel; r++)
				{
					// Calculate random pixel through the image plane
					float xOffset = -0.5f + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (0.5f - -0.5f)));
					float yOffset = -0.5f + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (0.5f - -0.5f)));

					float i = (2 * ((x + 0.5f + xOffset) * widthInverse) - 1) * angle * aspectRatio;
					float j = (1 - 2 * ((y + 0.5f + yOffset) * heightInverse)) * angle;
					glm::vec3 ray = glm::normalize(glm::vec3(i, j, -1));

					// Calculate the point hit on the focus plane for depth of field
					float distance = m_focalDistance / fabs(ray.z);
					glm::vec3 focusPlanePoint = ray * distance;

					// Calculate random origin on the lens
					float radius = static_cast <float>(rand()) / static_cast <float>(RAND_MAX);
					float theta = (static_cast <float>(rand()) / static_cast <float>(RAND_MAX)) * 2.0f * 3.14f;
					glm::vec3 originOnLens = m_lensRadius * glm::vec3(radius * cos(theta), radius * sin(theta), 0);

					glm::vec3 focusedRay = focusPlanePoint - originOnLens;

					// Transform the ray and origin from camera space into world space.
					glm::vec4 temp = m_cameraToWorld * glm::vec4(focusedRay, 0);
					focusedRay = glm::normalize(glm::vec3(temp.x, temp.y, temp.z));

					temp = m_cameraToWorld * glm::vec4(originOnLens, 1);
					originOnLens = { temp.x, temp.y, temp.z };

					// Cast the ray using the Scene class' recursive TraceRay method
					color += scene->TraceRay(originOnLens, focusedRay, glm::vec3(1.0f), 1);
				}

				color = color / float(m_raysPerPixel);

				// Clamp returned value just to be safe
				glm::vec3 clampedColor = color;//glm::clamp(color, { 0,0,0 }, { 1,1,1 });

				m_renderMutex.lock();

				// Store in the cimg object
				m_renderTarget(x, y, 0, 0) = (unsigned char)(clampedColor.r * 255);
				m_renderTarget(x, y, 0, 1) = (unsigned char)(clampedColor.g * 255);
				m_renderTarget(x, y, 0, 2) = (unsigned char)(clampedColor.b * 255);

				m_renderMutex.unlock();
			}
		}
	}
}