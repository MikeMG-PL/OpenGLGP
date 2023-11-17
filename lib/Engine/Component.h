#pragma once
#include <memory>

class GameObject;

class Component
{
public:

	const std::shared_ptr<GameObject>& GetParent() const;
	virtual void Start() {}
	virtual void Update() {}
	virtual void FixedUpdate() {}
	virtual ~Component() = default;
	std::shared_ptr<GameObject> parent;

private:


};
