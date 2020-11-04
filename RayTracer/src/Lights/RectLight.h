#pragma once

#include "../Core.h"
#include "AreaLight.h"
#include "../RTMath.h"

namespace RayTracer
{
	class RT_API RectLight : public AreaLight
	{
	public:
		RectLight(glm::mat4 transform, float width, float length, ConstMaterial* material);
		glm::vec3 SampleRadiance(const glm::vec3& point, const glm::vec3& normal, glm::vec3 &sampleDirection, float &pdf, float &sampleDistance);

	protected:
		virtual bool TestIntersection(const glm::vec3& rayOrigin, const glm::vec3& rayDirection, HitInfo& hitInfo);

	private:
		float m_length, m_width;
		glm::vec3 m_xAxis, m_yAxis, m_normal, m_center;
	};
}