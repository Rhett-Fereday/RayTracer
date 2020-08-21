#include "Sphere.h"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtx/transform.hpp"

namespace RayTracer
{
    Sphere::Sphere(glm::mat4 transform, float radius, Material material) : m_transform(transform), m_radius(radius), m_material(material)
    {
        m_inverseTransform = glm::inverse(m_transform);
    }

    bool Sphere::Intersects(const glm::vec3& rayOrigin, const glm::vec3& rayDirection, HitInfo& hitInfo)
    {
        glm::vec4 centerPos = m_transform * glm::vec4(0, 0, 0, 1);
        glm::vec3 center = glm::vec3(centerPos.x, centerPos.y, centerPos.z);

        float a = dot(rayDirection, rayDirection);
        float b = dot(2.0f * rayDirection, (rayOrigin - center));
        float c = dot(rayOrigin - center, rayOrigin - center) - (m_radius * m_radius);

        float radicand = b * b - 4 * a * c;

        if (radicand < 0) return false;

        float u1, u2;
        glm::vec3 intersect1, intersect2;
        u1 = (-b + sqrt(radicand)) / (2.0f * a);
        u2 = (-b - sqrt(radicand)) / (2.0f * a);

        if (u2 < u1) u1 = u2;

        // Account for floating point inaccuracy
        if (u1 < 0.0001) return false;

        hitInfo.hitPosition = rayOrigin + rayDirection * u1;
        hitInfo.hitNormal = glm::normalize(hitInfo.hitPosition - center);
        hitInfo.hitDistance = glm::distance(rayDirection, hitInfo.hitPosition);

        glm::vec4 temp = m_inverseTransform * glm::vec4(hitInfo.hitNormal, 0 );
        glm::vec3 surfacePoint = glm::vec3(temp.x, temp.y, temp.z);

        /*float u = (1 + atan2(surfacePoint.z, surfacePoint.x) / 3.14) * 0.5;
        float v = acosf(surfacePoint.y) / 3.14;*/

        float phi = atan2(surfacePoint.z, surfacePoint.x);
        float theta = asin(surfacePoint.y);
        float u = 1 - (phi + 3.14) / (2 * 3.14);
        float v = (theta + 3.14 / 2) / 3.14;

		hitInfo.hitColor = m_material.GetAlbedo(u, v);

        return true;
    }
}