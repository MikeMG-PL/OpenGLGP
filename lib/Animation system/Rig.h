#pragma once
#include <vector>

#include "AnimXForm.h"
#include "Components/Transform.h"
#include "Engine/HashedString.h"

class Rig
{
public:

	std::vector<hstring> boneNames;
	std::vector<int> parents;
	std::vector<glm::mat4> inverseBindPose;
	std::vector<glm::mat4> skinnedPose;
	unsigned int numBones = 0;
	void LocalToModel(std::vector<xform>& modelPose, const std::vector<xform>& localPose);
	void MultiplyInverseBindPoseByModelPose(const std::vector<xform>& modelPose);

};
