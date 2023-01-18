#include "Light.h"
namespace RayTracer
{
	Light::Light(glm::mat4 transform, vec3 color, float intensity, int nSamples)
		: transform(transform), inverseTransform(glm::inverse(transform)),
		color(color), intensity(intensity), nSamples(nSamples)
	{
		auto worldPos = transform * glm::vec4( 0, 0, 0, 1 );
		position = { worldPos.x, worldPos.y, worldPos.z };
	}
}