#pragma once
#include <vector>
#include "Components/Transform.h"
#include "Engine/HashedString.h"

class Rig
{
public:

	std::vector<hstring> boneNames;
	std::vector<int> parents;
	std::vector<glm::mat4> bindPose;
	std::vector<glm::mat4> skinnedPose;
	unsigned int numBones;
	void LocalToModel(std::vector<glm::mat4>& modelPose, const std::vector<glm::mat4>& localPose);
	void MultiplyInverseBindPoseByModelPose(const std::vector<glm::mat4>& modelPose);

};
