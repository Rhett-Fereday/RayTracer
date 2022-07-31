#pragma once

#include "../../Core.h"
#include "PostProcess.h"
#include <vector>
#include <string>

namespace RayTracer
{
	class RT_API PostProcessGroup : public PostProcess
	{
	public:
		PostProcessGroup(std::string groupName);
		void AddPostProcess(shared_ptr<PostProcess> postProcess);
		vector<vector<vec3>> Apply(const vector<vector<vec3>>& imageBuffer) override;
		std::string GroupName();

	private:
		vector<shared_ptr<PostProcess>> processes;
		const std::string groupName;
	};
}