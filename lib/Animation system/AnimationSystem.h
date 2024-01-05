#pragma once
class AnimationSystem
{
public:

	AnimationSystem& operator=(const AnimationSystem&) = delete;
	AnimationSystem(const AnimationSystem&) = delete;
	static AnimationSystem& Get();

	void Update();

private:

	AnimationSystem() = default;
};

