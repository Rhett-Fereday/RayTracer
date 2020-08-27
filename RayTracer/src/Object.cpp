#include "Object.h"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtx/transform.hpp"

namespace RayTracer
{
	Object::Object(glm::mat4 transform, ConstMaterial* material) : m_transform(transform), m_material(material)
	{
		m_inverseTransform = glm::inverse(m_transform);
	}
}