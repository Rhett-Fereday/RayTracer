//#pragma once
//
//#include "../Core.h"
//#include "Light.h"
//#include "glm/glm.hpp"
//
//namespace RayTracer
//{
//	class RT_API SpotLight : public Light
//	{
//	public:
//		SpotLight(glm::vec3 color, float intensity, glm::vec3 position, glm::vec3 direction, float innerAngle, float outerAngle = 0.0f);
//		glm::vec3 SampleRadiance(const glm::vec3& point, const glm::vec3& normal, glm::vec3 &sampleDirection, float &pdf, float &sampleDistance);
//
//	private:
//		glm::vec3 m_position, m_direction;
//		float m_innerAngle, m_outerAngle;
//	};
//}