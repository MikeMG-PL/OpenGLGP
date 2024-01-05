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
	// For some reason, probably order of operations, this loop cannot be here. It should be either in main rendering loop
	// or I should use my Brand New Rendering Injector



	// auto allGameObjects = GameInstance::Get().allGameObjects;

	// for (const auto& gameObjectPtr : allGameObjects)
	// {
	// 	if (const auto riggedModelComponent = gameObjectPtr->GetComponent<RiggedModel>())
	// 	{
	// 		const glm::mat4 model = gameObjectPtr->GetTransform()->modelMatrix;
	// 		const glm::mat4 view = Renderer::Get().GetViewMatrix();
	// 		const glm::mat4 projection = Renderer::Get().GetProjectionMatrix();
	//
	// 		Renderer::Get().GetShader().use();
	// 		Renderer::Get().GetShader().setMat4("projection", projection);
	// 		Renderer::Get().GetShader().setMat4("view", view);
	// 		Renderer::Get().GetShader().setMat4("model", model);
	//
	// 		Rig rig = riggedModelComponent->rig;
	// 		rig.LocalToModel(riggedModelComponent->modelPose, riggedModelComponent->localPose);
	// 		rig.MultiplyInverseBindPoseByModelPose(riggedModelComponent->modelPose);
	//
	// 		riggedModelComponent->Draw(Renderer::Get().GetShader());
	// 	}
	// }
}
