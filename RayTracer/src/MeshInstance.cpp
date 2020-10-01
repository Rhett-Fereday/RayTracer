#include "MeshInstance.h"

namespace RayTracer
{
	MeshInstance::MeshInstance(Mesh * mesh, glm::mat4 transform, ConstMaterial * material) : Object(transform, material), m_mesh(mesh)
	{
	}

	bool MeshInstance::Intersects(const glm::vec3 & rayOrigin, const glm::vec3 & rayDirection, HitInfo & hitInfo)
	{
		glm::vec4 tempOrigin = m_inverseTransform * glm::vec4(rayOrigin, 1);
		glm::vec4 tempDirection = m_inverseTransform * glm::vec4(rayDirection, 0);
		glm::vec3 transformedOrigin = { tempOrigin.x, tempOrigin.y, tempOrigin.z };
		glm::vec3 transformedDirection = { tempDirection.x, tempDirection.y, tempDirection.z };

		if (m_mesh->Intersects(transformedOrigin, transformedDirection, hitInfo))
		{
			hitInfo.hitPosition = rayOrigin + rayDirection * hitInfo.hitDistance;
			hitInfo.hitMaterial = m_material;

			glm::vec4 tempNormal = m_inverseTransposeTransform * glm::vec4(hitInfo.hitNormal, 0);
			hitInfo.hitNormal = glm::normalize(glm::vec3(tempNormal.x, tempNormal.y, tempNormal.z));

			return true;
		}

		return false;
	}
}