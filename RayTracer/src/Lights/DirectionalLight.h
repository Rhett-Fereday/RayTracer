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

        glm::vec3 SampleRadiance(const glm::vec3& point, const glm::vec3& normal, glm::vec3 &sampleDirection, float &pdf, float &sampleDistance);

    private:
        glm::vec3 m_direction;
    };
}