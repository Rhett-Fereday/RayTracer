#include "AreaLight.h"

RayTracer::AreaLight::AreaLight(glm::mat4 transform, ConstMaterial * material) : Object(transform, material), Light(material->albedo, material->emissiveStrength)
{
}
