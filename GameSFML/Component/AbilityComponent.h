#pragma once
#include <vector>
#include "entt/entt.hpp"
#include "GameplayTag.h"
#include "AttributeFactory.h"
#include "box2d/box2d.h"
namespace GAS
{
	
	struct AbilityComponent
	{
		entt::entity owner;
		entt::hashed_string abilityName;
	};

	using WaitTargetDataObserver = entt::observer;
	using WaitLocationObserver = entt::observer;
	using WaitTargetData = entt::entity;
	using WaitLocation = b2Vec2;

	struct TryActivateAbility {};
	using TryActivateDelegate = entt::delegate<bool(entt::entity, entt::registry&)>;
	using ActivateBehaviorDelegate = entt::delegate<void(entt::entity, entt::registry&)>;
	using CostDelegate = entt::delegate<void(entt::entity, entt::registry&)>;
	
	struct TagCollection
	{
		Tag::Bitfiled tags;
		// Cancels any already-executing Ability with Tags matching the list provided while this Ability is executing.
		Tag::Bitfiled cancelWithTag;
		// Prevents execution of any other Ability with a matching Tag while this Ability is executing.
		Tag::Bitfiled blockWithTag;
		//While this Ability is executing, the owner of the Ability will be granted this set of Tags.
		Tag::Bitfiled act_OwnedTags;
		//The Ability can only be activated if the activating Actor or Component has all of these Tags.
		Tag::Bitfiled act_RequiredTags;
		//The Ability can only be activated if the activating Actor or Component does not have any of these Tags.
		Tag::Bitfiled act_BlockedTags;
		//The Ability can only be activated if the target Actor or Component does not have any of these Tags.
		Tag::Bitfiled target_RequiredTags;
		// Same as Target Required Tags but with Blocked tags.
		Tag::Bitfiled target_BlockedTags;
	};
	enum BehaviorComponent
	{
		PASSIVE = 1 << 1, //Cannot be cast like above but this one shows up on the ability HUD.
		NO_TARGET = 1 << 2, //Doesn't need a target to be cast, ability fires off as soon as the button is pressed.
		UNIT_TARGET = 1 << 3, //Needs a target to be cast on.
		POINT = 1 << 4, //Can be cast anywhere the mouse cursor is (if a unit is clicked it will just be cast where the unit was standing).
		AOE = 1 << 5, //Draws a radius where the ability will have effect. Kinda like POINT but with a an area of effect display.
		CHANNELLED = 1 << 6, //Channeled ability. If the user moves or is silenced the ability is interrupted.
		TOGGLE = 1 << 7, //Can be insta-toggled.
	};

	struct CostComponent
	{
		entt::hashed_string attributeName;
		float amount;
	};
	struct CooldownComponent
	{
		float maxTime = 0.0f;
		float curTime = 0.0f;
	};
	enum EventTrigger
	{
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
}
