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
		m_rawBuffer.resize(height);
		m_postProcessBuffer.resize(height);

		for (int i = 0; i < height; i++)
		{
			m_rawBuffer[i].resize(width);
			m_postProcessBuffer[i].resize(width);
		}

		m_renderTarget.assign(m_width, m_height, 1, 3);

        m_worldToCamera = glm::lookAt(position, lookAt, upVector);
        m_cameraToWorld = glm::inverse(m_worldToCamera);
		m_raysPerPixel = 4;
		m_numberOfThreads = std::thread::hardware_concurrency();

		m_focalDistance = glm::distance(position, lookAt); // Set focus plane to the point we want to look at
	}
	
	void Camera::AddPostProcessGroup(PostProcessGroup* processGroup)
	{
		m_postProcessGroups.push_back(processGroup);
	}

    // Cast the camera rays into the scene. Code set up from scratchapixel.com with modifications to support camera transforms
	void Camera::Render(Scene* scene)
	{
		for (int y = 0; y < m_height; y++)
		{
			for (int x = 0; x < m_width; x++)
			{
				// Add each pixel to the work queue
				m_renderQueue.push(std::make_pair(x, y));
			}
		}

		srand(static_cast <unsigned> (time(0)));
		std::thread *threads = new std::thread[m_numberOfThreads - 1];

		// Spawn off the worker threads
		for (int i = 0; i < m_numberOfThreads - 1; i++)
		{
			threads[i] = std::thread(&Camera::ThreadRender, this, scene);
		}

		// Main thread should also do work
		ThreadRender(scene);

		// Wait for all threads to be finished
		for (int t = 0; t < m_numberOfThreads - 1; t++)
		{
			threads[t].join();
		}
	}
	
	cimg_library::CImg<unsigned char> Camera::GetRenderTarget()
	{
		return m_renderTarget;
	}

	void Camera::SaveRender(const char* filename)
	{
		const char* extension = ".ppm";
		std::string outputName;

		for (int i = 0; i < m_postProcessGroups.size(); i++)
		{
			outputName.clear();
			outputName.append(filename);
			outputName.append(m_postProcessGroups[i]->GroupName());
			outputName.append(extension);

			m_postProcessGroups[i]->Apply(&m_rawBuffer, &m_postProcessBuffer, m_width, m_height);

			// Place final result in the cimg render target
			for (int y = 0; y < m_height; y++)
			{
				for (int x = 0; x < m_width; x++)
				{
					glm::vec3 color = m_postProcessBuffer[y][x].color;

					unsigned char r = glm::clamp((unsigned char)(color.r * 255), (unsigned char)0, (unsigned char)255);
					unsigned char g = glm::clamp((unsigned char)(color.g * 255), (unsigned char)0, (unsigned char)255);
					unsigned char b = glm::clamp((unsigned char)(color.b * 255), (unsigned char)0, (unsigned char)255);

					int R = color.r * 255;
					int G = color.g * 255;
					int B = color.b * 255;

					if (R > 255) R = 255;
					if (R < 0) R = 0;

					if (G > 255) G = 255;
					if (G < 0) G = 0;

					if (B > 255) B = 255;
					if (B < 0) B = 0;

					m_renderTarget(x, y, 0, 0) = (unsigned char)R;
					m_renderTarget(x, y, 0, 1) = (unsigned char)G;
					m_renderTarget(x, y, 0, 2) = (unsigned char)B;
				}
			}

			m_renderTarget.save(outputName.c_str());
		}
	}

	void Camera::ThreadRender(Scene * scene)
	{
		float widthInverse = 1.0f / (float)m_width;
		float heightInverse = 1.0f / (float)m_height;
		float fieldOfView = m_fov;
		float aspectRatio = (float)m_width / (float)m_height;
		float angle = tan(3.14 * 0.5f * fieldOfView / 180.0f);

		// Run until work is finished
		while (!m_renderQueue.empty())
		{
			m_renderMutex.lock();

			// pull a pixel from the work queue
			std::pair<int, int> pixel = m_renderQueue.front();
			m_renderQueue.pop();

			m_renderMutex.unlock();

			int x = pixel.first;
			int y = pixel.second;

			glm::vec3 color = { 0,0,0 };
			GBufferInfo info;
			info.albedo = { 0,0,0 }; info.color = { 0,0,0 }; info.normal = { 0,0,0 }; info.position = { 0,0,0 };

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
				GBufferInfo tempInfo = scene->TraceRay(originOnLens, focusedRay, glm::vec3(1.0f), 1);
				
				info.albedo += tempInfo.albedo;
				info.color += tempInfo.color;
				info.normal += tempInfo.normal;
				info.position += tempInfo.position;
			}

			color = color / float(m_raysPerPixel);

			info.albedo = info.albedo / float(m_raysPerPixel);
			info.color = info.color / float(m_raysPerPixel);
			info.normal = glm::normalize(info.normal / float(m_raysPerPixel));
			info.position = info.position / float(m_raysPerPixel);

			m_renderMutex.lock();

			// Store color in the raw buffer
			m_rawBuffer[y][x] = info;

			m_renderMutex.unlock();
		}
	}
}