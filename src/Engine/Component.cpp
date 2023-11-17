#include "Engine/Component.h"
#include "Engine/GameObject.h"
#include <memory>

const std::shared_ptr<GameObject>& Component::GetParent() const
{
	return parent;
}
