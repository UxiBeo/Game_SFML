#pragma once
#include <vector>
#include "entt/entt.hpp"
#include "box2d/box2d.h"
#include "AttributeComponent.h"
#include "GameplayTag.h"
namespace GAS
{
	struct AbilitySlot
	{
		std::vector<entt::entity> abilities;
	};
	struct AbilityTag
	{
		Tag::Bitfiled source_RequiredTags;
		Tag::Bitfiled source_BlockTags;
		Tag::Bitfiled onStart_Source_GrandTags;
		Tag::Bitfiled onStart_Source_RemoveTags;
		Tag::Bitfiled onEnd_Source_GrandTags;
		Tag::Bitfiled onEnd_Source_RemoveTags;
	};
	struct AbilityComponent
	{
		entt::entity owner = entt::null;
		entt::entity self = entt::null;
		entt::delegate<void(const AbilityComponent&, entt::registry&)> mrD;
	};
	struct ModifiedEffect
	{
		std::vector<entt::entity> eEffect;
	};
	struct TryActivateAbility {};
	struct CostComponent
	{
		GES::AttributeType attributeName;
		float amount = 0.0f;
	};
	struct CommitAbility{};
	struct DoingCooldown{};
	struct CooldownComponent
	{
		float maxTime = 0.0f;
		float curTime = 0.0f;
	};
	
	enum class Event
	{
		OnAnimationNotify,
		OnAbilityInterrupt,
		OnAbilityEndChannel,
		OnAbilityPhaseStart, //Triggers when the ability is cast(before the unit turns toward the target)
		OnAbilityStart,
		OnAttack,
		OnAttackAllied,
		OnAttackFailed,
		OnChannelFinish,
		OnChannelInterrupted,
		OnChannelSucceeded,
		OnCreated,
		OnEquip,
		OnHealReceived,
		OnHealthGained,
		OnHeroKilled,
		OnManaGained,
		OnOrder,
		OnOwnerDied,
		OnOwnerSpawned,
		OnProjectileDodge,
		OnProjectileFinish,
		OnProjectileHitUnit, // Adding the KV pair "DeleteOnHit" "0" in this block will cause the projectile to not disappear when it hits a unit.
		OnRespawn,
		OnSpellStart,
		OnSpentMana,
		OnStateChanged,
		OnTeleported,
		OnTeleporting,
		OnToggleOff,
		OnToggleOn,
		OnUnitMoved,
		OnUpgrade,
	};
	

	struct Activating {};
}
