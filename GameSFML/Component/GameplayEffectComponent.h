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
		Tag::Bitfiled target_RequiredTags = 0;
		Tag::Bitfiled target_BlockTags = 0;
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
		uint8_t value = 0;
	};

	/*struct EffectResource
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
							resource->CostAmount.emplace_back(GES::ManaPoint, i["Amount"].get<float>());
						}
						else if(attName == 6)
						{
							resource->CostAmount.emplace_back(GES::HealthPoint, i["Amount"].get<float>());
						}
					}
				}
				if (json.count("Modified") > 0)
				{
					for (auto i : json["Modified"])
					{
						Value value;
						value.baseValue = i["Value"]["Base"].get<float>();
						value.baseMultiplier = i["Value"]["MBase"].get<float>();
						value.value = i["Value"]["Value"].get<float>();
						value.baseMultiplier = i["Value"]["MValue"].get<float>();
						auto type = i["Type"].get<uint8_t>();
						switch (type)
						{
						case 1:
							resource->Modified.emplace_back(CritChange, value);
							break;
						case 2:
							resource->Modified.emplace_back(CritDame, value);
							break;
						case 3:
							resource->Modified.emplace_back(Attack, value);
							break;
						case 4:
							resource->Modified.emplace_back(Defence, value);
							break;
						case 5:
							resource->Modified.emplace_back(HealthPoint, value);
							break;
						case 6:
							resource->Modified.emplace_back(ManaPoint, value);
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
		std::vector<std::pair<AttributeType, float>> CostAmount;
		std::vector<std::pair<AttributeType, Value>> Modified;
	};*/
	struct TryAppyEffect
	{
		entt::entity target = entt::null;
		entt::entity creator = entt::null;
		entt::entity source = entt::null;
		entt::entity self = entt::null;
		entt::entity prefapEntity = entt::null;
	};
	struct OnEffectCreate
	{
		EffectTags tags;
		entt::delegate<void(const TryAppyEffect&, entt::registry&)> mrD;
	};
	struct RenewEffect
	{
		entt::entity entity = entt::null;
		uint8_t times = 0;
	};
	struct Executions
	{
		uint8_t value = 0;
	};

	struct ModifiedAttribute
	{
		struct Total
		{
			AttributeType type;
			Value modValue;
			Value storeValue;
		};
		std::vector<Total> value;
	};

	struct ModifiedCurrentSpecial
	{
		std::array<float, 2> value{ 0.0f };
	};
	struct OutputDame
	{
		float value = 0.0f;
	};
	struct CurrentActiveEffect
	{
		std::unordered_map<entt::entity, entt::entity> entities;
	};
	
	struct MarkDelete{};
};