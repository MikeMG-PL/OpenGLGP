#include "Components/SpotLight.h"
#include "Engine/GameObject.h"
#include "Engine/Renderer.h"
#include "Helpers/MathHelpers.h"

void SpotLight::Start()
{
	RenderInjector::Start();

	shader = std::make_shared<Shader>(Renderer::Get().GetShader());
	Renderer::Get().RegisterSpotLight(shared_from_this());
	ID = Renderer::Get().GetSpotLights()->size() - 1;

	uniform << "spotLights[" << ID << "].";
	positionName << uniform.str() << "position";
	directionName << uniform.str() << "direction";
	ambientName << uniform.str() << "ambient";
	diffuseName << uniform.str() << "diffuse";
	specularName << uniform.str() << "specular";
	constantName << uniform.str() << "constant";
	linearName << uniform.str() << "linear";
	quadraticName << uniform.str() << "quadratic";
	cutOffName << uniform.str() << "cutOff";
	outerCutOffName << uniform.str() << "outerCutOff";
}

void SpotLight::RenderUpdate()
{
	RenderInjector::RenderUpdate();

	position = GetParent()->GetTransform()->localPosition;
	direction = GetParent()->GetTransform()->localEulerAngles;

	shader->setVector3(positionName.str(), position);
	shader->setVector3(directionName.str(), eulerToVector(direction));
	shader->setVector3(ambientName.str(), { 0.0f, 0.0f, 0.0f } );
	shader->setVector3(diffuseName.str(), { 1.0f, 1.0f, 1.0f });
	shader->setVector3(specularName.str(), { 1.0f, 1.0f, 1.0f } );
	shader->setFloat(constantName.str(), 1.0f);
	shader->setFloat(linearName.str(), 0.09f);
	shader->setFloat(quadraticName.str(), 0.032f);
	shader->setFloat(cutOffName.str(), glm::cos(glm::radians(12.5f)));
	shader->setFloat(outerCutOffName.str(), glm::cos(glm::radians(15.0f)));
}
