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
		float frameTime = 0.0f;
		uint8_t iBegin = 0;
		uint8_t iEnd =  0;
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
			resource->width = Json["width"].get<unsigned int>();
			resource->height = Json["height"].get<unsigned int>();
			resource->tileWidth = Json["tileWidth"].get<unsigned int>();
			resource->tileHeight = Json["tileHeight"].get<unsigned int>();
			resource->nFrame = Json["frames"].get<uint8_t>() - 1u;
			unsigned int nCot = resource->width / resource->tileWidth;
			resource->AddAnimation(Json["r"], nCot);
			resource->AddAnimation(Json["rd"], nCot);
			resource->AddAnimation(Json["d"], nCot);
			resource->AddAnimation(Json["ld"], nCot);
			resource->AddAnimation(Json["l"], nCot);
			resource->AddAnimation(Json["lu"], nCot);
			resource->AddAnimation(Json["u"], nCot);
			resource->AddAnimation(Json["ru"], nCot);
			return resource;
		}
	};
	using Loader = AnimationLoader;
	void AddAnimation(const nlohmann::json& set, unsigned int nCot)
	{
		AnimationSet as;
		as.iBegin = (uint8_t)frames.size();
		const auto iStart = set["index"].get<uint8_t>();
		for (auto i = iStart; i < iStart + nFrame; i++)
		{
			unsigned int iHang = i / nCot;
			unsigned int iCot = i % nCot;
			frames.emplace_back(iCot * tileWidth, iHang * tileHeight,
				tileWidth, tileHeight);
		}
		as.iEnd = (uint8_t)frames.size() - 1u;
		as.frameTime = set["frameTime"].get<float>() / 1000.0f;
		sets.push_back(as);
	}
	std::vector<AnimationSet> sets;
	std::vector<sf::IntRect> frames;
	entt::hashed_string textureName;
	uint8_t nFrame = 0u;
	unsigned int width, height, tileWidth, tileHeight;
};

struct AnimationComponent
{
	AnimationComponent(entt::hashed_string animationName)
		:
		ar(&Codex<AnimationResource>::Retrieve(animationName))
	{
		frameTime = ar->sets.begin()->frameTime;
		iBegin = ar->sets.begin()->iBegin;
		iEnd = iBegin;
		iCurrent = iBegin;
	}
	const AnimationResource* ar = nullptr;
	uint8_t iSet = 0;
	bool isUpdate = true;
	float frameTime = 0.16f;
	float curTime = 0.0f;
	uint8_t iCurrent = 0u;
	uint8_t iBegin = 0u;
	uint8_t iEnd = 0u;
};
struct AnimNotify
{
	float triggerTime;
	entt::delegate<void(entt::entity, entt::registry&)> notifyDelegate;
};