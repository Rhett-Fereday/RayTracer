#pragma once

#include "../Core.h"
#include "glm/glm.hpp"
#include "Light.h"

namespace RayTracer
{
    class RT_API DirectionalLight : public Light
    {
    public:
        DirectionalLight(glm::vec3 direction = { 0,-1,0 }, glm::vec3 color = { 1,1,1 }, float intensity = 1.0f);

        glm::vec3 Illumination(const glm::vec3& point, const glm::vec3& normal, const glm::vec3& ray);
        glm::vec3 DirectionToLight(const glm::vec3& point);
		float DistanceToLight(const glm::vec3 & point);

    private:
        glm::vec3 m_direction;
    };
}