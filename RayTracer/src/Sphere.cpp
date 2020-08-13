#include "Sphere.h"

namespace RayTracer
{
    RayTracer::Sphere::Sphere(glm::mat4 transform, float radius, glm::vec3 albedo) : m_transform(transform), m_radius(radius), m_albedo(albedo)
    {
    }

    bool Sphere::Intersects(const glm::vec3& rayOrigin, const glm::vec3& rayDirection, HitInfo& hitInfo, glm::mat4 cameraTransform)
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

        if (u1 < 0.0001) return false;

        hitInfo.hitPosition = rayOrigin + rayDirection * u1;
        hitInfo.hitNormal = glm::normalize(hitInfo.hitPosition - center);
        hitInfo.hitColor = m_albedo;

        return true;
    }
}