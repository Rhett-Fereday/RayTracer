#pragma once

#include "Core.h"
#include "glm/glm.hpp"

namespace RayTracer
{
    class RT_API DirectionalLight
    {
    public:
        DirectionalLight(glm::vec3 direction = { 0,1,0 }, glm::vec3 color = { 1,1,1 }, float intensity = 1.0f);

        glm::vec3 illumination(const glm::vec3& point, const glm::vec3& normal, const glm::vec3& ray);

    private:
        glm::vec3 m_direction, m_color;
        float m_intensity;
    };
}