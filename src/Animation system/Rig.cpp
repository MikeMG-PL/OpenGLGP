#include "Animation system/Rig.h"

void Rig::LocalToModel(std::vector<glm::mat4>& modelPose, const std::vector<glm::mat4>& localPose)
{
	modelPose = localPose;
	numBones = std::min(modelPose.size(), localPose.size());

	for (int i = 0; i < numBones; i++)
	{
		const int idParent = parents[i];
		if (idParent >= 0)
		{
			// There might be something wrong here
			modelPose[i] = modelPose[idParent] * localPose[i];
		}
		else
			modelPose[i] = localPose[i];
	}

	// Remember to: inv bind pose * pose in model space = skinning matrices
}

void Rig::MultiplyInverseBindPoseByModelPose(const std::vector<glm::mat4>& modelPose)
{
	for(int i = 0; i < numBones; i++)
	{
		skinnedPose[i] = inverse(bindPose[i]) * modelPose[i];
	}
}
