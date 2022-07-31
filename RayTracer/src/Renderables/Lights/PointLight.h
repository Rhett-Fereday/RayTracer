//#pragma once
//
//#include "../Core.h"
//#include "Light.h"
//#include "glm/glm.hpp"
//
//namespace RayTracer
//{
//	class RT_API PointLight : public Light
//	{
//	public:
//		PointLight(glm::vec3 color, float intensity, glm::vec3 position);
//		glm::vec3 SampleRadiance(const glm::vec3& point, const glm::vec3& normal, glm::vec3 &sampleDirection, float &pdf, float &sampleDistance);
//
//	private:
//		glm::vec3 m_position;
//	};
//}