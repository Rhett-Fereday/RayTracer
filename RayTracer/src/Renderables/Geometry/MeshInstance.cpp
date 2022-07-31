//#include "MeshInstance.h"
//
//namespace RayTracer
//{
//	MeshInstance::MeshInstance(Mesh * mesh, glm::mat4 transform, ConstMaterial * material) : Object(transform, material), m_mesh(mesh)
//	{
//	}
//
//	bool MeshInstance::TestIntersection(const glm::vec3 & rayOrigin, const glm::vec3 & rayDirection, HitInfo & hitInfo)
//	{
//		if (!m_mesh->Intersects(rayOrigin, rayDirection, hitInfo)) return false;		
//
//		return true;
//	}
//}