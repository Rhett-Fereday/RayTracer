#pragma once

#include "../Core.h"
#include "Object.h"
#include "Mesh.h"
#include "glm/glm.hpp"
#include "../Material.h"

namespace RayTracer
{
	class RT_API MeshInstance : public Object
	{
	public:
		MeshInstance(Mesh* mesh, glm::mat4 transform, ConstMaterial* material);

	protected:
		virtual bool TestIntersection(const glm::vec3& rayOrigin, const glm::vec3& rayDirection, HitInfo& hitInfo);

	private:
		Mesh* m_mesh;
	};
}