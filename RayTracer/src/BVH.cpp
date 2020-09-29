#include "BVH.h"
#include "Mesh.h"
#include "tiny_obj_loader.h"
#include <math.h>

namespace RayTracer
{
	BVH::BVH(Mesh* mesh, int depth) : m_depth(depth)
	{
		m_mesh = mesh;
		m_firstLeafIndex = pow(2, depth) - 1;

		m_volumes.resize(pow(2, depth + 1) - 1, nullptr);

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
	int BVH::AddVolume(int parentIndex, BoundingVolume * volume)
	{
		// Root volume
		if (parentIndex == -1)
		{
			// Root already exists
			if (m_volumes[0] != nullptr) return -1;

			// Set root
			m_volumes[0] = volume;
			return 0;
		}

		// Calculate non-root index (left child)
		int index = 2 * parentIndex + 1;

		// Don't actually care about left/right just see if this spot is available
		if (m_volumes[index] == nullptr)
		{
			m_volumes[index] = volume;
			return index;
		}

		index++;

		// Left wasn't available; check right
		if (m_volumes[index] == nullptr)
		{
			m_volumes[index] = volume;
			return index;
		}

		// Could not add volume
		return -1;
	}

	bool BVH::Intersects(const glm::vec3 & rayOrigin, const glm::vec3 & rayDirection, HitInfo &hitInfo)
	{
		float hitDistance;
		//// Test highest level bounding volume
		//if (!m_volumes[0]->Intersects(rayOrigin, rayDirection, hitDistance)) return false;

		// We hit the top level, must recurse down hierarchy
		return IntersectionTraversal(0, rayOrigin, rayDirection, hitInfo);

		/*float closestHit = 1000000.0f;
		bool returnValue = false;

		for (int i = m_firstLeafIndex; i < m_volumes.size(); i++)
		{			
			for (int j = 0; j < m_volumes[i]->m_indices->size(); j++)
			{
				std::vector<std::pair<int, int>>* indices = m_volumes[i]->m_indices;
				TriangleData triangle = m_mesh->GetTriangleData((*indices)[j].first, (*indices)[j].second);
				HitInfo tempInfo;

				if (m_mesh->RayTriangleIntersect(rayOrigin, rayDirection, triangle, tempInfo))
				{					
					if (tempInfo.hitDistance < closestHit)
					{
						returnValue = true;
						closestHit = tempInfo.hitDistance;
						hitInfo = tempInfo;
					}
				}
			}
		}

		return returnValue;*/
	}

	bool BVH::IntersectionTraversal(int currentIndex, const glm::vec3 & rayOrigin, const glm::vec3 & rayDirection, HitInfo &hitInfo)
	{
		float hitDistance;

		// Didn't hit the volume
		if (!m_volumes[currentIndex]->Intersects(rayOrigin, rayDirection, hitDistance)) return false;

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
		std::vector<std::pair<int, int>>* indices = m_volumes[volumeIndex]->m_indices;
		float closestHit = 1000000.0f;
		bool returnValue = false;

		for (int i = 0; i < indices->size(); i++)
		{
			TriangleData triangle = m_mesh->GetTriangleData((*indices)[i].first, (*indices)[i].second);
			HitInfo tempInfo;

			if (m_mesh->RayTriangleIntersect(rayOrigin, rayDirection, triangle, tempInfo))
			{
				if (tempInfo.hitDistance < closestHit)
				{
					returnValue = true;
					closestHit = tempInfo.hitDistance;
					hitInfo = tempInfo;
				}
			}
		}

		return returnValue;
	}

	void BVH::BuildHierarchy(Mesh* mesh, glm::vec3 splitAxis, int parentIndex, const glm::vec3 minDimensions, const glm::vec3 maxDimensions)
	{
		// Create the volume at this level

		BoundingVolume* volume = new BoundingVolume(minDimensions, maxDimensions);
		int storedIndex = AddVolume(parentIndex, volume);

		// At a leaf, so add Indices and end recursion
		if (storedIndex >= m_firstLeafIndex)
		{
			std::vector<tinyobj::shape_t>* shapes = mesh->m_shapes;
			std::vector<std::pair<int, int>>* leafIndices = new std::vector<std::pair<int, int>>();

			// Loop over the shapes in the mesh
			for (int i = 0; i < shapes->size(); i++)
			{
				// Loop over all of the triangles in the mesh of this shape
				for (int j = 0; j < (*shapes)[i].mesh.indices.size(); j += 3)
				{
					TriangleData triangle = mesh->GetTriangleData(i, j);

					if (BoundingVolume::IntersectsTriangle(volume, &triangle)) leafIndices->push_back(std::make_pair(i, j));
				}
			}

			volume->m_indices = leafIndices;
			return;
		}

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