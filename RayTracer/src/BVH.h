#pragma once

#include "Objects/Mesh.h"
#include "RTMath.h"
#include <vector>

namespace RayTracer
{
	class BVH
	{
	public:
		BVH(Mesh* mesh, int depth);
		int AddVolume(int parentIndex, RTMath::AABB* volume);
		bool Intersects(const glm::vec3& rayOrigin, const glm::vec3& rayDirection, HitInfo &hitInfo);

	private:
		Mesh* m_mesh;
		int m_depth, m_firstLeafIndex;
		std::vector<std::pair<RTMath::AABB*, std::vector<std::pair<int,int>>*>> m_volumes;

		bool IntersectionTraversal(int currentIndex, const glm::vec3 & rayOrigin, const glm::vec3 & rayDirection, HitInfo &hitInfo);
		bool IntersectVolumeTriangles(int volumeIndex, const glm::vec3 & rayOrigin, const glm::vec3 & rayDirection, HitInfo &hitInfo);
		void BuildHierarchy(Mesh* mesh, glm::vec3 splitAxis, int parentIndex, const glm::vec3 minDimensions, const glm::vec3 maxDimensions);
	};
}