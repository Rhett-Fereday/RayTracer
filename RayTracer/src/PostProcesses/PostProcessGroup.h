#pragma once

#include "../Core.h"
#include "PostProcess.h"
#include <vector>

namespace RayTracer
{
	class RT_API PostProcessGroup : public PostProcess
	{
	public:
		PostProcessGroup(const char* groupName);
		void AddPostProcess(PostProcess* postProcess);
		void Apply(std::vector<std::vector<GBufferInfo>>* inputImage, std::vector<std::vector<GBufferInfo>>* outputImage, int width, int height);
		const char* GroupName();

	private:
		std::vector<PostProcess*> m_processes;
		const char* m_groupName;
	};
}