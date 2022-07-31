#include "Light.h"
namespace RayTracer
{
	Light::Light(glm::mat4 transform, vec3 color, float intensity, int nSamples)
		: transform(transform), inverseTransform(glm::inverse(transform)),
		color(color), intensity(intensity), nSamples(nSamples)
	{
	}
}