#include "AreaLight.h"

RayTracer::AreaLight::AreaLight(glm::mat4 transform, ConstMaterial * material, glm::vec3 color, float intensity) : Object(transform, material), Light(color, intensity)
{
}
