//#include "RectLight.h"
//#include "../RTMath.h"
//#include <cstdlib>
//#include <ctime>
//#include <algorithm>
//
//namespace RayTracer
//{
//	RectLight::RectLight(glm::mat4 transform, float width, float length, ConstMaterial * material)
//		: AreaLight(transform, material), m_width(width), m_length(length)
//	{
//		glm::vec4 tempXAxis = m_transform * glm::vec4(1, 0, 0, 0);
//		m_xAxis = glm::normalize(glm::vec3(tempXAxis.x, tempXAxis.y, tempXAxis.z));
//
//		glm::vec4 tempYAxis = m_transform * glm::vec4(0, 1, 0, 0);
//		m_yAxis = glm::normalize(glm::vec3(tempYAxis.x, tempYAxis.y, tempYAxis.z));
//
//		glm::vec4 temp = m_transform * glm::vec4(0, 0, 0, 1);
//		m_center = { temp.x, temp.y, temp.z };
//
//		glm::vec4 tempNormal = m_inverseTransposeTransform * glm::vec4(0, 0, 1, 0);
//		m_normal = glm::normalize(glm::vec3(tempNormal.x, tempNormal.y, tempNormal.z));
//	}
//
//	glm::vec3 RectLight::SampleRadiance(const glm::vec3& point, const glm::vec3& normal, glm::vec3 &sampleDirection, float &pdf, float &sampleDistance)
//	{
//		pdf = 1.0f / (m_width * m_length);
//
//		glm::vec3 returnRadiance = { 0,0,0 };
//
//		float xOffset = -0.5f + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (0.5f - -0.5f)));
//		xOffset = xOffset * m_width;
//
//		float yOffset = -0.5f + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (0.5f - -0.5f)));
//		yOffset = yOffset * m_length;
//
//		glm::vec3 samplePoint = m_center + xOffset * m_xAxis;
//		samplePoint = samplePoint + yOffset * m_yAxis;
//
//		sampleDirection = glm::normalize(samplePoint - point);
//
//		float outgoingIncidence(std::max(0.0f, glm::dot(m_normal, -sampleDirection)));
//		if (outgoingIncidence == 0.0f) return returnRadiance;
//
//		float incomingIncidence = std::max(0.0f, glm::dot(normal, sampleDirection));
//		if (incomingIncidence == 0.0f) return returnRadiance;
//
//		sampleDistance = glm::distance(samplePoint, point);
//		float lightReceived = m_intensity / (sampleDistance * sampleDistance);
//		returnRadiance = m_color * lightReceived * incomingIncidence * outgoingIncidence;
//
//		return returnRadiance;
//	}
//
//	bool RectLight::TestIntersection(const glm::vec3 & rayOrigin, const glm::vec3 & rayDirection, HitInfo & hitInfo)
//	{
//		// Plane faces positive z in model space
//		//float incidence = glm::dot(-rayDirection, { 0,0,1 });
//
//		// Parallel or behind
//		//if (incidence <= 0) return false;
//
//		float distance;
//		bool intersects = RTMath::RayPlaneIntersection(rayOrigin, rayDirection, m_width, m_length, distance);
//
//		if (!intersects) return false;
//
//		hitInfo.hitDistance = distance;
//		hitInfo.hitNormal = { 0,0,1 };
//		return true;
//	}
//}