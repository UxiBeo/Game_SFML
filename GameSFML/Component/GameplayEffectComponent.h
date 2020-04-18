#pragma once
#include <vector>
#include "../MaxxUtility.h"
#include "entt/entt.hpp"
#include "AttributeComponent.h"
#include "GameplayTag.h"
#include "../GameResource.h"
namespace GES
{
	
	struct EffectTags
	{
		Tag::Bitfiled tags;

		Tag::Bitfiled target_RequiredTags;
		Tag::Bitfiled target_BlockTags;

		Tag::Bitfiled begin_target_GrantTags;
		Tag::Bitfiled begin_target_RemoveTags;

		Tag::Bitfiled end_target_GrantTags;
		Tag::Bitfiled end_target_RemoveTags;
	};
	enum class DurationType
	{
		Passive,
		Onetime,
		LimitedTime
	};
	struct DurationPassive {};
	struct DurationOnetime{};
	struct DurationLimitedTime
	{
		float curTime = 0.0f;
		float maxTime = 0.0f;
	};

	//Interval Tick
	struct IntervalTick
	{
		float curTime = 0.0f;
		float intervalTime = 0.0f;
		uint8_t curTick = 0;
		uint8_t maxTick = 0;	
	};

	//Stack
	struct Stack
	{
		uint8_t minStack = 0;
		uint8_t curStack = 0;
		uint8_t maxStack = 0;
	};
	struct StackLost
	{
		float curTime = 0.0f;
		float maxTime = 0.0f;
	};
	struct AddStack
	{
		uint8_t value;
	};

	struct EffectResource
	{
		struct EffectLoader final : entt::loader<EffectLoader, EffectResource> {
			std::shared_ptr<EffectResource> load(entt::hashed_string filename) const
			{
				const nlohmann::json json = MaxxUtility::GetJson(filename);
				auto resource = std::make_shared<EffectResource>();

				std::string name = json["name"].get<std::string>();
				resource->effectName = entt::hashed_string(name.c_str());
				auto tags = resource->tags;
				unsigned int Time = 0;
				unsigned int IntervalTick = 0;
				unsigned int TickCapacity = 0;
				unsigned int minStack = 0;
				unsigned int maxStack = 0;
				
				if (json.count("Tags") > 0)
				{
					if (json["Tags"].count("Tag") > 0)
					{
						tags.tags |= json["Tags"]["Tag"].get<unsigned int>();
					}
					if (json["Tags"].count("target_RequiredTags") > 0)
					{
						tags.target_RequiredTags |= json["Tags"]["target_RequiredTags"].get<unsigned int>();
					}
					if (json["Tags"].count("target_BlockTags") > 0)
					{
						tags.target_BlockTags |= json["Tags"]["target_BlockTags"].get<unsigned int>();
					}
					if (json["Tags"].count("begin_target_GrantTags") > 0)
					{
						tags.begin_target_GrantTags |= json["Tags"]["begin_target_GrantTags"].get<unsigned int>();
					}
					if (json["Tags"].count("begin_target_RemoveTags") > 0)
					{
						tags.begin_target_RemoveTags |= json["Tags"]["begin_target_RemoveTags"].get<unsigned int>();
					}
					if (json["Tags"].count("end_target_GrantTags") > 0)
					{
						tags.end_target_GrantTags |= json["Tags"]["end_target_GrantTags"].get<unsigned int>();
					}
					if (json["Tags"].count("end_target_RemoveTags") > 0)
					{
						tags.end_target_RemoveTags |= json["Tags"]["end_target_RemoveTags"].get<unsigned int>();
					}
				}

				if (json.count("Duration") > 0)
				{
					auto type = json["Duration"]["Type"].get<uint8_t>();
					switch (type)
					{
					case 0:
						resource->durationType = DurationType::Passive;
						break;
					case 1:
						resource->durationType = DurationType::Onetime;
						break;
					case 2:
						resource->durationType = DurationType::LimitedTime;
						resource->durationTime = json["Duration"]["Time"].get<float>();
						break;
					}
				}

				if (json.count("IntervalTick") > 0)
				{
					resource->intervalTick = json["IntervalTick"].get<float>();
				}
				if (json.count("MaxTick") > 0)
				{
					resource->maxTick = json["MaxTick"].get<uint8_t>();
				}
				if (json.count("Stack") > 0)
				{
					resource->minStack = json["Stack"]["Min"].get<uint8_t>();
					resource->maxStack = json["Stack"]["Max"].get<uint8_t>();
				}
				if (json.count("Cost") > 0)
				{
					for (auto i : json["Cost"])
					{
						auto attName = i["Type"].get<int>();
						if (attName == 5)
						{
							resource->CostAmount.emplace_back(RPGS::ManaPoint, i["Amount"].get<float>());
						}
						else if(attName == 6)
						{
							resource->CostAmount.emplace_back(RPGS::HealthPoint, i["Amount"].get<float>());
						}
					}
				}
				if (json.count("Capture") > 0)
				{
					for (auto i : json["Capture"])
					{
						if (i["Type"].get<uint8_t>() == RPGS::Attack)
						{
							resource->Capture.emplace_back(RPGS::Attack);
						}
					}
				}
				if (json.count("Modified") > 0)
				{
					for (auto i : json["Modified"])
					{
						RPGS::Value value;
						value.baseValue = i["Value"]["Base"].get<float>();
						value.baseMultiplier = i["Value"]["MBase"].get<float>();
						value.value = i["Value"]["Value"].get<float>();
						value.baseMultiplier = i["Value"]["MValue"].get<float>();
						auto type = i["Type"].get<uint8_t>();
						switch (type)
						{
						case 1:
							resource->Modified.emplace_back(RPGS::CritChange, value);
							break;
						case 2:
							resource->Modified.emplace_back(RPGS::CritDame, value);
							break;
						case 3:
							resource->Modified.emplace_back(RPGS::Attack, value);
							break;
						case 4:
							resource->Modified.emplace_back(RPGS::Defence, value);
							break;
						case 5:
							resource->Modified.emplace_back(RPGS::HealthPoint, value);
							break;
						case 6:
							resource->Modified.emplace_back(RPGS::ManaPoint, value);
							break;
						default:
							break;
						}
						
					}
				}
				return resource;
			}
		};
		
		using Loader = EffectLoader;
		entt::hashed_string effectName;
		EffectTags tags;
		DurationType durationType = DurationType::Passive;
		float durationTime = 0.0f;
		float intervalTick = 0.0f;
		uint8_t maxTick = 0;
		uint8_t minStack = 0;
		uint8_t maxStack = 0;
		std::vector<std::pair<RPGS::AttributeType, float>> CostAmount;
		std::vector<RPGS::AttributeType> Capture;
		std::vector<std::pair<RPGS::AttributeType, RPGS::Value>> Modified;
	};

	struct EffectInfo
	{
		EffectTags tags;
		entt::hashed_string effectName;
		entt::hashed_string texturePath;
		entt::entity source = entt::null;
		entt::entity target = entt::null;
	};
	
	struct Executions
	{
		uint8_t value;
	};

	template <RPGS::AttributeType T>
	struct ModifiedValue
	{
		RPGS::Value modValue;
		RPGS::Value storeValue;
		entt::entity target = entt::null;
	};

	template <RPGS::AttributeType T>
	struct CostValue
	{
		float value;
		entt::entity target = entt::null;
	};

	template <RPGS::AttributeType T>
	struct OutputDame
	{
		float value;
		float capture = 0.0f;
		entt::entity target = entt::null;
	};
	template <RPGS::AttributeType T>
	struct CaptureAttack
	{
		entt::entity source = entt::null;
	};
	struct CurrentActiveEffect
	{
		std::set<entt::entity> entities;
	};
	struct TryAppyEffect
	{
		entt::entity target;
		entt::entity source;
		entt::hashed_string effectName;
	};
	struct MarkDelete{};
};