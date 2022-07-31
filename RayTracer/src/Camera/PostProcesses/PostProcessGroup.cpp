#include "PostProcessGroup.h"

namespace RayTracer
{
	PostProcessGroup::PostProcessGroup(std::string groupName) : groupName(groupName)
	{
	}

	void PostProcessGroup::AddPostProcess(shared_ptr<PostProcess> postProcess)
	{
		processes.push_back(postProcess);
	}

	vector<vector<vec3>> PostProcessGroup::Apply(const vector<vector<vec3>>& imageBuffer)
	{
		auto returnBuffer = imageBuffer;

		for (const auto& process : processes)
		{
			returnBuffer = process->Apply(returnBuffer);
		}

		return returnBuffer;
	}

	std::string PostProcessGroup::GroupName()
	{
		return groupName;
	}
}