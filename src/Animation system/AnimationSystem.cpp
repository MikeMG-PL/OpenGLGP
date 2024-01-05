#include "Animation system/AnimationSystem.h"
#include "Engine/GameObject.h"
#include "Animation system/RiggedModel.h"
#include "Engine/GameInstance.h"
#include "Engine/Renderer.h"

AnimationSystem& AnimationSystem::Get()
{
	static AnimationSystem instance;
	return instance;
}

void AnimationSystem::Update()
{
	auto allGameObjects = GameInstance::Get().allGameObjects;

	for (const auto& gameObjectPtr : allGameObjects)
	{
		if (const auto riggedModelComponent = gameObjectPtr->GetComponent<RiggedModel>())
		{
			Rig rig = riggedModelComponent->rig;
			rig.LocalToModel(riggedModelComponent->modelPose, riggedModelComponent->localPose);
			rig.MultiplyInverseBindPoseByModelPose(riggedModelComponent->modelPose);

			riggedModelComponent->Draw(Renderer::Get().GetShader());
		}
	}
}
