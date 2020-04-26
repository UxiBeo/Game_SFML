#pragma once
#include "SFML/Graphics.hpp"
#include "../Codex.h"
#include "entt/entt.hpp"
#include "../MaxxUtility.h"
#include "../GameResource.h"
#include <vector>
struct AnimationResource
{
	struct AnimationSet
	{
		uint16_t iBegin = 0;
		uint16_t iEnd =  0;
	};
	struct FrameRect
	{
		FrameRect(int x, int y, int w, int h, float px, float py)
			:
			rect(x,y,w,h),
			pivot(px,py)
		{}
		sf::IntRect rect;
		sf::Vector2f pivot;
	};
	struct AnimationLoader final : entt::loader<AnimationLoader, AnimationResource> {
		std::shared_ptr<AnimationResource> load(entt::hashed_string filename) const
		{
			const nlohmann::json Json = MaxxUtility::GetJson(filename);

			std::string imageName{ "Data\\Images\\" };
			imageName += Json["imageName"].get<std::string>();
			const entt::hashed_string textureName{ imageName.c_str() };
			Codex<TextureResource>::LoadFromFile(textureName);
			auto resource = std::make_shared<AnimationResource>();
			resource->textureName = textureName;
			resource->frameTime = Json["frameTime"].get<float>();
			resource->AddSet(Json["frames"]["run"]);
			resource->AddAnimation(Json["frames"]["idle"]);
			resource->AddSet(Json["frames"]["attack"]);
			return resource;
		}
	};
	using Loader = AnimationLoader;
	void AddSet(const nlohmann::json& set, bool isdir = true)
	{
		if (isdir)
		{
			AddAnimation(set["r"]);
			AddAnimation(set["dr"]);
			AddAnimation(set["d"]);
			AddAnimation(set["dl"]);
			AddAnimation(set["l"]);
			AddAnimation(set["ul"]);
			AddAnimation(set["u"]);
			AddAnimation(set["ur"]);
			return;
		}
		
		
	}
	void AddAnimation(const nlohmann::json& set)
	{
		AnimationSet as;
		as.iBegin = (uint16_t)frames.size();
		for (auto& f : set)
		{
			frames.emplace_back(f["x"].get<int>(), f["y"].get<int>(), f["w"].get<int>(), f["h"].get<int>() ,
				f["px"].get<float>(), f["py"].get<float>() );
		}
		as.iEnd = (uint16_t)frames.size() - 1u;
		sets.push_back(as);
	}
	std::vector<AnimationSet> sets;
	std::vector<FrameRect> frames;
	entt::hashed_string textureName;
	float frameTime = 0.0f;
};

struct AnimationComponent
{
	AnimationComponent(entt::hashed_string animationName)
		:
		ar(&Codex<AnimationResource>::Retrieve(animationName))
	{
		frameTime = ar->frameTime;
		iBegin = ar->sets.begin()->iBegin;
		iEnd = ar->sets.begin()->iEnd;
		iCurrent = iBegin;
	}
	const AnimationResource* ar = nullptr;
	uint8_t iSet = 0;
	float frameTime = 0.16f;
	float curTime = 0.0f;
	uint16_t iCurrent = 0u;
	uint16_t iBegin = 0u;
	uint16_t iEnd = 0u;
};
struct AnimNotify
{
	float triggerTime = 0.6f;
	float curTime = 0.0f;
	entt::entity listenerE = entt::null;
	entt::delegate<void(const entt::entity&, const entt::entity&, entt::registry&)> triggerD;
	entt::delegate<void(const entt::entity&, const entt::entity&, entt::registry&)> interrupD;
};
struct PlayAnimationMontage
{
	uint8_t iSet;
	AnimNotify an;
};
struct MontagePlaying {};