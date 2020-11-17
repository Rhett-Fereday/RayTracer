#include "PostProcessGroup.h"

namespace RayTracer
{
	PostProcessGroup::PostProcessGroup(const char* groupName)
	{
		m_groupName = groupName;
	}

	void PostProcessGroup::AddPostProcess(PostProcess * postProcess)
	{
		m_processes.push_back(postProcess);
	}

	void PostProcessGroup::Apply(std::vector<std::vector<GBufferInfo>>* inputImage, std::vector<std::vector<GBufferInfo>>* outputImage, int width, int height)
	{
		for (int i = 0; i < m_processes.size(); i++)
		{
			if(i == 0) m_processes[i]->Apply(inputImage, outputImage, width, height);
			else m_processes[i]->Apply(outputImage, outputImage, width, height);
		}
	}

	const char * PostProcessGroup::GroupName()
	{
		return m_groupName;
	}
}