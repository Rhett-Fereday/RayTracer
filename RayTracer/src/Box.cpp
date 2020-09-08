#include "Box.h"

namespace RayTracer
{
	Box::Box(glm::mat4 transform, ConstMaterial* material, glm::vec3 minDimensions, glm::vec3 maxDimensions) : Object(transform, material), m_minDimensions(minDimensions), m_maxDimensions(maxDimensions)
	{
	}
	
	bool Box::Intersects(const glm::vec3& rayOrigin, const glm::vec3& rayDirection, HitInfo& hitInfo)
	{
		{
			// Intersection method from Real-Time Rendering and Essential Mathematics for Games

			float tMin = 0.0f;
			float tMax = 100000.0f;

			glm::vec3 OBBposition_worldspace(m_transform[3].x, m_transform[3].y, m_transform[3].z);

			glm::vec3 delta = OBBposition_worldspace - rayOrigin;

			// Test intersection with the 2 planes perpendicular to the OBB's X axis
			{
				glm::vec3 xaxis(m_transform[0].x, m_transform[0].y, m_transform[0].z);
				float e = glm::dot(xaxis, delta);
				float f = glm::dot(rayDirection, xaxis);
				glm::vec3 tempNormal = xaxis;

				if (fabs(f) > 0.001f)
				{ // Standard case

					float t1 = (e + m_minDimensions.x) / f; // Intersection with the "left" plane
					float t2 = (e + m_maxDimensions.x) / f; // Intersection with the "right" plane
					// t1 and t2 now contain distances betwen ray origin and ray-plane intersections

					// We want t1 to represent the nearest intersection, 
					// so if it's not the case, invert t1 and t2
					if (t1 > t2)
					{
						float w = t1;t1 = t2;t2 = w; // swap t1 and t2
						tempNormal = -tempNormal;
					}

					// tMax is the nearest "far" intersection (amongst the X,Y and Z planes pairs)
					if (t2 < tMax)
						tMax = t2;
					// tMin is the farthest "near" intersection (amongst the X,Y and Z planes pairs)
					if (t1 > tMin)
					{
						tMin = t1;
						hitInfo.hitNormal = -tempNormal;
					}

					// And here's the trick :
					// If "far" is closer than "near", then there is NO intersection.
					// See the images in the tutorials for the visual explanation.
					if (tMax < tMin)
						return false;

				}
				else
				{ // Rare case : the ray is almost parallel to the planes, so they don't have any "intersection"
					if (-e + m_minDimensions.x > 0.0f || -e + m_maxDimensions.x < 0.0f)
						return false;
				}
			}


			// Test intersection with the 2 planes perpendicular to the OBB's Y axis
			// Exactly the same thing than above.
			{
				glm::vec3 yaxis(m_transform[1].x, m_transform[1].y, m_transform[1].z);
				float e = glm::dot(yaxis, delta);
				float f = glm::dot(rayDirection, yaxis);
				glm::vec3 tempNormal = yaxis;

				if (fabs(f) > 0.001f)
				{

					float t1 = (e + m_minDimensions.y) / f;
					float t2 = (e + m_maxDimensions.y) / f;

					if (t1 > t2) 
					{ 
						float w = t1;t1 = t2;t2 = w; 
						tempNormal = -tempNormal;
					}

					if (t2 < tMax)
						tMax = t2;
					if (t1 > tMin)
					{
						tMin = t1;
						hitInfo.hitNormal = -tempNormal;
					}
					if (tMin > tMax)
						return false;

				}
				else
				{
					if (-e + m_minDimensions.y > 0.0f || -e + m_maxDimensions.y < 0.0f)
						return false;
				}
			}


			// Test intersection with the 2 planes perpendicular to the OBB's Z axis
			// Exactly the same thing than above.
			{
				glm::vec3 zaxis(m_transform[2].x, m_transform[2].y, m_transform[2].z);
				float e = glm::dot(zaxis, delta);
				float f = glm::dot(rayDirection, zaxis);
				glm::vec3 tempNormal = zaxis;

				if (fabs(f) > 0.001f)
				{

					float t1 = (e + m_minDimensions.z) / f;
					float t2 = (e + m_maxDimensions.z) / f;

					if (t1 > t2) 
					{ 
						float w = t1;t1 = t2;t2 = w; 
						tempNormal = -tempNormal;
					}

					if (t2 < tMax)
						tMax = t2;
					if (t1 > tMin)
					{
						tMin = t1;
						hitInfo.hitNormal = -tempNormal;
					}
					if (tMin > tMax)
						return false;

				}
				else
				{
					if (-e + m_minDimensions.z > 0.0f || -e + m_maxDimensions.z < 0.0f)
						return false;
				}
			}

			hitInfo.hitNormal = glm::normalize(hitInfo.hitNormal);
			hitInfo.hitMaterial = m_material;
			hitInfo.hitDistance = tMin;
			hitInfo.hitPosition = rayOrigin + rayDirection * tMin;
			return true;
		}


	}
}