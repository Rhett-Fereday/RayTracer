#pragma once

#include "../../Core.h"

namespace RayTracer
{
	class Interaction;
	class SurfaceInteraction;

	// Base class to allow storing all light types in a single vector
	class Light
	{
	public:
		Light(glm::mat4 transform, vec3 color, float intensity, int nSamples = 1);

		virtual vec3 L(const SurfaceInteraction& interaction, const vec3& w) const = 0;
		virtual vec3 SampleLi(const Interaction& interaction, const vec2& u, vec3& wi, float &pdf, float& sampleDistance) const = 0;
		virtual float PdfLi(const Interaction& interaction, const vec3& wi) const = 0;

		virtual float Power() const = 0;
		inline vec3 Position() const { return position; };

		inline int NumSamples() const { return nSamples; };

	protected:
		glm::mat4 transform, inverseTransform;
		vec3 color;
		float intensity;
		int nSamples;
		vec3 position;
	};
}