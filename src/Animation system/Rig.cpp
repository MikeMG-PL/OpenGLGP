#include "Animation system/Rig.h"
#include "Helpers/aiHelpers.h"
#include "Animation system/AnimXForm.h"
#include "glm/gtx/matrix_decompose.hpp"
#include "glm/gtx/quaternion.hpp"

void Rig::LocalToModel(std::vector<xform>& modelPose, const std::vector<xform>& localPose)
{
	modelPose = localPose;
	numBones = std::min(modelPose.size(), localPose.size());

	for (int i = 0; i < numBones; i++)
	{
		const int idParent = parents[i];
		if (idParent >= 0)
		{
			// There might be something wrong here: ORDER of multiplication, parent indices or bones
			// might not be sorted by parent
			modelPose[i] = modelPose[idParent] * localPose[i];
		}
		else
			modelPose[i] = localPose[i];
	}

	// Remember to: inv bind pose * pose in model space = skinning matrices
}

void Rig::MultiplyInverseBindPoseByModelPose(const std::vector<xform>& modelPose)
{
	for(int i = 0; i < numBones; i++)
	{
		xform inverseBindPoseXForm, skinnedXForm;
		glm::vec3 scale;
		glm::quat rotation;
		glm::vec3 translation;
		glm::vec3 skew;
		glm::vec4 perspective;
		glm::decompose(inverseBindPose[i], scale, rotation, translation, skew, perspective);

		inverseBindPoseXForm.position = translation;
		inverseBindPoseXForm.rotation = rotation;

		skinnedXForm = inverseBindPoseXForm * modelPose[i];

		glm::mat4 skinnigMatrix = glm::mat4(1.0f);
		skinnigMatrix = glm::translate(skinnigMatrix, skinnedXForm.position);
		skinnigMatrix = skinnigMatrix * glm::toMat4(skinnedXForm.rotation);

		skinnedPose[i] = skinnigMatrix;
	}
}
