#pragma once
#include <iostream>
#include <memory>
#include <vector>

#include "GameInstance.h"
#include "Components/Transform.h"

class Component;

class GameObject : public std::enable_shared_from_this<GameObject>
{
public:

	GameObject();
	static std::shared_ptr<GameObject> CreateObject();
	std::vector<std::shared_ptr<Component>> GetComponents();

	// Returns first occurence of a component of a <T> type (no safety for adding only one component yet)
	template <typename T>
	std::shared_ptr<T> GetComponent() const
	{
		for (const auto& component : components) {
			std::shared_ptr<T> typedComponent = std::dynamic_pointer_cast<T>(component);
			if (typedComponent) {
				return typedComponent;
			}
		}
		return nullptr;
	}

	std::shared_ptr<Transform> GetTransform();

	template <class T, typename... TArgs>
	std::shared_ptr<T> AddComponent(TArgs&&... args)
	{
		auto component = std::make_shared<T>(std::forward<TArgs>(args)...);
		GameInstance::Get().allComponents.emplace_back(component);
		components.emplace_back(component);
		component->parent = shared_from_this();
		component->Start();
		return component;
	}

	//void RemoveComponent(const Component& component);

private:

	std::vector<std::shared_ptr<Component>> components;
	std::shared_ptr<Transform> transform;
};
