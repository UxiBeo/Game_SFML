#pragma once
#include "SFML/Graphics.hpp"
#include "../GameResource.h"
#include "entt/signal/sigh.hpp"
#include <vector>
struct AnimationComponent
{
	AnimationComponent(entt::hashed_string animationName)
		:
		animationName(animationName)
	{
		const auto& resource = Codex<AnimationResource>::Retrieve(animationName);
		textureName = resource.textureName;
		holdTime = float(resource.frameTime) / 1000.0f;
		
		beginFrame = resource.sets[0].rangeIndex.first;
		endFrame = resource.sets[0].rangeIndex.second;
	}
	bool isUpdate = true;
	float holdTime = 0.16f;
	float curFrameTime = 0.0f;
	unsigned char iCurFrame = 0u;
	unsigned char beginFrame = 0u;
	unsigned char endFrame = 0u;
	entt::hashed_string animationName;
	entt::hashed_string textureName;
};
using PairKeyframe = std::pair<unsigned char, entt::delegate<void(entt::entity, entt::registry&)>>;
using AnimNotify = std::vector<PairKeyframe>;