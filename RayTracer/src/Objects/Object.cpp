#include "Object.h"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtx/transform.hpp"

namespace RayTracer
{
	Object::Object(glm::mat4 transform, ConstMaterial* material) : m_transform(transform), m_material(material)
	{
		m_inverseTransform = glm::inverse(m_transform);
		m_inverseTransposeTransform = glm::transpose(m_inverseTransform);
	}

	bool Object::Intersects(const glm::vec3 & rayOrigin, const glm::vec3 & rayDirection, HitInfo & hitInfo)
	{
		glm::vec4 tempOrigin = m_inverseTransform * glm::vec4(rayOrigin, 1);
		glm::vec4 tempDirection = m_inverseTransform * glm::vec4(rayDirection, 0);
		glm::vec3 transformedOrigin = { tempOrigin.x, tempOrigin.y, tempOrigin.z };
		glm::vec3 transformedDirection = { tempDirection.x, tempDirection.y, tempDirection.z };

		if(!TestIntersection(transformedOrigin, transformedDirection, hitInfo)) return false;

		hitInfo.hitMaterial = m_material;
		hitInfo.hitPosition = rayOrigin + rayDirection * hitInfo.hitDistance;

		glm::vec4 tempNormal = m_inverseTransposeTransform * glm::vec4(hitInfo.hitNormal, 0);
		hitInfo.hitNormal = glm::normalize(glm::vec3(tempNormal.x, tempNormal.y, tempNormal.z));

		return true;
	}
}