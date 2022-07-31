#include "AreaLight.h"
#include "../../Interactions/SurfaceInteraction.h"

namespace RayTracer
{
	AreaLight::AreaLight(glm::mat4 transform, shared_ptr<Geometry> geometry, vec3 color, float totalPower, int nSamples)
		: Light(transform, color, totalPower, nSamples), geometry(geometry)
	{
	}

	vec3 AreaLight::L(const SurfaceInteraction& interaction, const vec3& w) const
	{
		return glm::dot(interaction.geometricNormal, w) > 0.0f ? color * intensity : vec3(0);
		//return glm::max(glm::dot(interaction.geometricNormal, w), 0.0f) * color * intensity;
	}

	vec3 AreaLight::SampleLi(const Interaction& interaction, const vec2& u, vec3& wi, float& pdf, float& sampleDistance) const
	{
		auto lightInteraction = geometry->Sample(interaction, u);

		wi = glm::normalize(lightInteraction.p - interaction.p);
		sampleDistance = glm::distance(lightInteraction.p, interaction.p);
		pdf = geometry->Pdf(interaction);

		return L(lightInteraction, -wi);
	}

	float AreaLight::PdfLi(const Interaction& interaction, const vec3& wi) const
	{
		return geometry->Pdf(interaction, wi);
	}

	vec3 AreaLight::Power() const
	{
		return intensity * color * geometry->Area();
	}
}