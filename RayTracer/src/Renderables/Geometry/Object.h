//#pragma once
//
//#include "../ConstMaterial.h"
//#include "../HitInfo.h"
//#include "glm/glm.hpp"
//
//namespace RayTracer
//{
//	// Base object class to allow storing all object types in a single vector
//	class Object
//	{
//	public:
//		Object(glm::mat4 transform, ConstMaterial* material);
//		bool Intersects(const glm::vec3& rayOrigin, const glm::vec3& rayDirection, HitInfo& hitInfo);
//
//	protected:
//		ConstMaterial* m_material;
//		glm::mat4 m_transform, m_inverseTransform, m_inverseTransposeTransform;
//
//		virtual bool TestIntersection(const glm::vec3& rayOrigin, const glm::vec3& rayDirection, HitInfo& hitInfo) = 0;
//	};
//}