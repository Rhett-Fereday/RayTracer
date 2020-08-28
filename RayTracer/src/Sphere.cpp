#include "Sphere.h"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtx/transform.hpp"

namespace RayTracer
{
    Sphere::Sphere(glm::mat4 transform, ConstMaterial* material, float radius) : Object(transform, material),  m_radius(radius)
    {
        glm::vec4 temp = m_transform * glm::vec4(0, 0, 0, 1);
        m_center = glm::vec3(temp.x, temp.y, temp.z);
    }

    bool Sphere::Intersects(const glm::vec3& rayOrigin, const glm::vec3& rayDirection, HitInfo& hitInfo)
    {
        float a = 1.0f;//dot(rayDirection, rayDirection);
        float b = dot(2.0f * rayDirection, (rayOrigin - m_center));
        float c = dot(rayOrigin - m_center, rayOrigin - m_center) - (m_radius * m_radius);

        float radicand = b * b - 4 * a * c;

        if (radicand < 0) return false;

        float u1, u2;
        glm::vec3 intersect1, intersect2;
        float root = sqrt(radicand);
        u1 = (-b + root) / (2.0f * a);
        u2 = (-b - root) / (2.0f * a);

        if (u2 < u1) u1 = u2;

        // Account for floating point inaccuracy
        if (u1 < 0.0001) return false;

        hitInfo.hitPosition = rayOrigin + rayDirection * u1;
        hitInfo.hitNormal = glm::normalize(hitInfo.hitPosition - m_center);
        hitInfo.hitDistance = glm::distance(rayDirection, hitInfo.hitPosition);

        glm::vec4 temp = m_inverseTransform * glm::vec4(hitInfo.hitNormal, 0 );
        glm::vec3 surfacePoint = glm::vec3(temp.x, temp.y, temp.z);

        /*float u = (1 + atan2(surfacePoint.z, surfacePoint.x) / 3.14) * 0.5;
        float v = acosf(surfacePoint.y) / 3.14;*/

        float phi = atan2(surfacePoint.z, surfacePoint.x);
        float theta = asin(surfacePoint.y);
        float u = 1 - (phi + 3.14) / (2 * 3.14);
        float v = (theta + 3.14 / 2) / 3.14;

		hitInfo.hitMaterial = m_material;

        return true;
    }
}