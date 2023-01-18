#include "Geometry.h"
#include "../../Interactions/SurfaceInteraction.h"
#include "../../Ray.h"

namespace RayTracer
{
	Geometry::Geometry(glm::mat4 transform) : transform(transform)
	{
		transformIsIdentity = (transform == glm::mat4(1.0));
		inverseTransform = glm::inverse(transform);
		inverseTransposeTransform = glm::transpose(inverseTransform);
	}

	bool Geometry::Intersects(const Ray& ray, float& tHit, shared_ptr<SurfaceInteraction> interaction) const
	{
		glm::vec4 tempOrigin = inverseTransform * glm::vec4(ray.origin, 1);
		glm::vec4 tempDirection = inverseTransform * glm::vec4(ray.direction, 0);
		glm::vec3 transformedOrigin = { tempOrigin.x, tempOrigin.y, tempOrigin.z };
		glm::vec3 transformedDirection = { tempDirection.x, tempDirection.y, tempDirection.z };

		Ray r(transformedOrigin, transformedDirection, ray.tMax);

		if (!TestIntersection(r, tHit, interaction)) return false;

		interaction->p = ray.origin + ray.direction * tHit;
		interaction->wo = -ray.direction;

		glm::vec4 tempNormal = inverseTransposeTransform * glm::vec4(interaction->geometricNormal, 0);
		interaction->geometricNormal = glm::normalize(glm::vec3(tempNormal.x, tempNormal.y, tempNormal.z));

		return true;
	}

	SurfaceInteraction Geometry::Sample(const Interaction& interaction, const vec2& u, float& pdf) const
	{
		auto geomInteraction = this->Sample(u);
		pdf = Pdf(geomInteraction);

		vec3 wi = glm::normalize(geomInteraction.p - interaction.p);

		auto distance = glm::distance(geomInteraction.p, interaction.p);

		pdf *= (distance * distance) / glm::abs(glm::dot(geomInteraction.geometricNormal, -wi));

		return geomInteraction;
	}

	float Geometry::Pdf(const Interaction& interaction) const
	{
		return 1.0f / Area();
	}

	float Geometry::Pdf(const Interaction& interaction, const vec3& wi) const
	{
		Ray ray(interaction.p, wi);

		float tHit;
		auto lightInteraction = std::make_shared<SurfaceInteraction>();

		if (!Intersects(ray, tHit, lightInteraction)) return 0.0f;

		auto distance = glm::distance(lightInteraction->p, interaction.p);
		auto denominator = glm::abs(glm::dot(lightInteraction->geometricNormal, -wi)) * Area();

		return (distance * distance) / denominator;
	}
}