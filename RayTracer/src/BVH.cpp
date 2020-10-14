#include "BVH.h"
#include "Objects/Mesh.h"
#include "tiny_obj_loader.h"
#include <math.h>

namespace RayTracer
{
	BVH::BVH(Mesh* mesh, int depth) : m_depth(depth)
	{
		m_mesh = mesh;
		m_firstLeafIndex = pow(2, depth) - 1;

		m_volumes.resize(pow(2, depth + 1) - 1, std::make_pair(nullptr,nullptr));

		glm::vec3 minDimensions = { 100000, 100000, 100000 };
		glm::vec3 maxDimensions = { -100000, -100000, -100000 };

		// Find the total model dimensions
		for (int i = 0; i < mesh->m_attrib->vertices.size(); i += 3)
		{
			glm::vec3 vertex = { mesh->m_attrib->vertices[i], mesh->m_attrib->vertices[i + 1], mesh->m_attrib->vertices[i + 2] };

			if (vertex.x < minDimensions.x) minDimensions.x = vertex.x;
			if (vertex.x > maxDimensions.x) maxDimensions.x = vertex.x;

			if (vertex.y < minDimensions.y) minDimensions.y = vertex.y;
			if (vertex.y > maxDimensions.y) maxDimensions.y = vertex.y;

			if (vertex.z < minDimensions.z) minDimensions.z = vertex.z;
			if (vertex.z > maxDimensions.z) maxDimensions.z = vertex.z;
		}

		BuildHierarchy(mesh, { 1,0,0 }, -1, minDimensions, maxDimensions);
	}

	// Add a volume into the implicit binary tree. Returns index at which it was added
	int BVH::AddVolume(int parentIndex, RTMath::AABB * volume)
	{
		// Root volume
		if (parentIndex == -1)
		{
			// Root already exists
			if (m_volumes[0].first != nullptr) return -1;

			// Set root
			m_volumes[0].first = volume;
			return 0;
		}

		// Calculate non-root index (left child)
		int index = 2 * parentIndex + 1;

		// Don't actually care about left/right just see if this spot is available
		if (m_volumes[index].first == nullptr)
		{
			m_volumes[index].first = volume;
			return index;
		}

		index++;

		// Left wasn't available; check right
		if (m_volumes[index].first == nullptr)
		{
			m_volumes[index].first = volume;
			return index;
		}

		// Could not add volume
		return -1;
	}

	bool BVH::Intersects(const glm::vec3 & rayOrigin, const glm::vec3 & rayDirection, HitInfo &hitInfo)
	{
		// We hit the top level, must recurse down hierarchy
		return IntersectionTraversal(0, rayOrigin, rayDirection, hitInfo);
	}

	bool BVH::IntersectionTraversal(int currentIndex, const glm::vec3 & rayOrigin, const glm::vec3 & rayDirection, HitInfo &hitInfo)
	{	
		RTMath::AABB* volume = m_volumes[currentIndex].first;

		float tempDistance;
		glm::vec3 tempNormal;
		bool tempInside;
		// Didn't hit the volume
		if (!RTMath::RayAABBIntersection(rayOrigin, rayDirection, *volume, tempNormal, tempDistance, tempInside)) return false;

		// Leaf volume, check triangles
		if (currentIndex >= m_firstLeafIndex)
		{
			return IntersectVolumeTriangles(currentIndex, rayOrigin, rayDirection, hitInfo);
		}

		// There are children, so test them
		int leftChildIndex = 2 * currentIndex + 1;
		int rightChildIndex = 2 * currentIndex + 2;
		HitInfo leftHitInfo, rightHitInfo;
		leftHitInfo.hitDistance = 1000000.0f; rightHitInfo.hitDistance = 1000000.0f;

		bool hitLeftChild = IntersectionTraversal(leftChildIndex, rayOrigin, rayDirection, leftHitInfo);
		bool hitRightChild = IntersectionTraversal(rightChildIndex, rayOrigin, rayDirection, rightHitInfo);

		// Neither recursive traversal hit anything
		if (!hitLeftChild && !hitRightChild) return false;

		// Hit left
		if (hitLeftChild) hitInfo = leftHitInfo;

		// Hit right and it was closer
		if (hitRightChild && (rightHitInfo.hitDistance < leftHitInfo.hitDistance)) hitInfo = rightHitInfo;

		return true;
	}

	bool BVH::IntersectVolumeTriangles(int volumeIndex, const glm::vec3 & rayOrigin, const glm::vec3 & rayDirection, HitInfo & hitInfo)
	{
		std::vector<std::pair<int, int>>* indices = m_volumes[volumeIndex].second;
		float closestHit = 1000000.0f;
		bool returnValue = false;

		for (int i = 0; i < indices->size(); i++)
		{
			RTMath::Triangle triangle = m_mesh->GetTriangleData((*indices)[i].first, (*indices)[i].second);
			glm::vec3 tempNormal;
			float tempDistance = 1000000.0f;
			bool tempInside;

			if (RTMath::RayTriangleIntersection(rayOrigin,rayDirection, triangle, tempNormal, tempDistance, tempInside))
			{
				if (tempDistance < closestHit)
				{
					returnValue = true;
					closestHit = tempDistance;
					hitInfo.hitDistance = tempDistance;
					hitInfo.hitNormal = tempNormal;
					hitInfo.insideObject = tempInside;
				}
			}
		}

		return returnValue;
	}

	void BVH::BuildHierarchy(Mesh* mesh, glm::vec3 splitAxis, int parentIndex, const glm::vec3 minDimensions, const glm::vec3 maxDimensions)
	{
		// Create the volume at this level

		RTMath::AABB* volume = new RTMath::AABB();
		volume->minDimensions = minDimensions;
		volume->maxDimensions = maxDimensions;

		int storedIndex = AddVolume(parentIndex, volume);

		// If it's the root volume we need to add all indices in the mesh
		if (parentIndex == -1)
		{
			std::vector<tinyobj::shape_t>* shapes = mesh->m_shapes;
			std::vector<std::pair<int, int>>* indicesInVolume = new std::vector<std::pair<int, int>>();

			// Loop over the shapes in the mesh
			for (int i = 0; i < shapes->size(); i++)
			{
				// Loop over all of the triangles in the mesh of this shape
				for (int j = 0; j < (*shapes)[i].mesh.indices.size(); j += 3)
				{
					indicesInVolume->push_back(std::make_pair(i, j));
				}
			}

			m_volumes[storedIndex].second = indicesInVolume;
		}
		// Otherwise we need to see which indices of the parent volume fall in this volume
		else
		{
			std::vector<std::pair<int, int>>* parentIndices = m_volumes[parentIndex].second;
			std::vector<std::pair<int, int>>* indicesInVolume = new std::vector<std::pair<int, int>>();

			for (int i = 0; i < parentIndices->size(); i++)
			{
				std::pair<int, int> indexData = (*parentIndices)[i];
				RTMath::Triangle triangle = mesh->GetTriangleData(indexData.first, indexData.second);

				if (RTMath::TriangleAABBIntersection(*volume, triangle)) indicesInVolume->push_back(std::make_pair(indexData.first, indexData.second));
			}

			m_volumes[storedIndex].second = indicesInVolume;
		}

		// Leaf volume, break recursion
		if (storedIndex >= m_firstLeafIndex) return;

		// Not a leaf, so create child volumes
		glm::vec3 currentDimensions = maxDimensions - minDimensions;
		glm::vec3 newDimensions = currentDimensions / 2.0f;
		glm::vec3 dimensionOffset = newDimensions * splitAxis;

		// New dimensions of 'left' child
		glm::vec3 leftMinDimensions = minDimensions;
		glm::vec3 leftMaxDimensions = maxDimensions - dimensionOffset;

		// New dimensions of 'right' child
		glm::vec3 rightMinDimensions = minDimensions + dimensionOffset;
		glm::vec3 rightMaxDimensions = maxDimensions;

		glm::vec3 newSplitAxis = { splitAxis.z, splitAxis.x, splitAxis.y };

		// Recurse on children
		BuildHierarchy(mesh, newSplitAxis, storedIndex, leftMinDimensions, leftMaxDimensions);
		BuildHierarchy(mesh, newSplitAxis, storedIndex, rightMinDimensions, rightMaxDimensions);
	}
}