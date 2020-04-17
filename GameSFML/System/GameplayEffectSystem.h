#pragma once
#include "../System/ISystemECS.h"
#include "../Component/GameplayEffectComponent.h"
class GameplayEffectSystem final : public ISystemECS
{
	void Update(entt::registry& ECS) final;
	void BeginPlay(entt::registry& ECS) final;
private:
	void DeleteEffect(entt::registry& ECS) const;
	void IntervalTickSystem(entt::registry& ECS, float dt) const;
	void DurationSystem(entt::registry& ECS, float dt) const;
	void StackSystem(entt::registry& ECS, float dt) const;
	void ExecutionSystem(entt::registry& ECS) const;
	void TryApplyEffect(entt::registry& ECS) const;

	template <RPGS::AttributeType T>
	void ModifiedAttribute(entt::registry& ECS) const
	{
		ECS.view<GES::Executions, GES::ModifiedValue<T>>().each([&ECS](auto entity, const GES::Executions& ec, GES::ModifiedValue<T>& mv) {
			auto& ta = ECS.get<RPGS::ModifiedAttribute<T>>(mv.target);
			auto value = mv.modValue * (float)ec.value;
			ta.value += value;
			mv.storeValue -= value;
			});
	}
	template <RPGS::AttributeType T>
	void ApplyCost(entt::registry& ECS) const
	{
		ECS.view<GES::Executions, GES::CostValue<T>> ().each([&ECS](auto entity, const GES::Executions& ec, const GES::CostValue<T>& mv) {
			ECS.get<RPGS::ModifiedConsumeCur<T>>(mv.target).value += mv.value * (float)ec.value;
			});
	}
	template <RPGS::AttributeType T>
	void ApplyDame(entt::registry& ECS) const
	{
		ECS.view<GES::Executions, GES::OutputDame<T>> ().each([&ECS](auto entity, const GES::Executions& ec, const GES::OutputDame<T>& mv) {
			ECS.get<RPGS::InputDame<T>>(mv.target).value += mv.value* (float)ec.value;
			});
	}
	template <RPGS::AttributeType T>
	void RestoreAttribute(entt::registry& ECS) const
	{
		ECS.view<GES::MarkDelete, const GES::ModifiedValue<T>>().each([&ECS](auto entity, auto, const GES::ModifiedValue<T>& mv) {
			ECS.get<RPGS::Attribute<T>>(mv.target).value += mv.storeValue;
			});
	}
	template <RPGS::AttributeType T>
	void CaptureAttack(entt::registry& ECS) const
	{
		ECS.view<GES::CaptureAttack<T>, GES::OutputDame<T>>().each([&ECS](auto entity, const GES::CaptureAttack<T>& ct, GES::OutputDame<T>& od) {
			if (auto* ta = ECS.try_get<RPGS::Attribute<T>>(ct.source); ta)
			{
				od.capture == ta->value.getFinalValue();
			}
			});
		ECS.clear<GES::CaptureAttack<T>>();
	}
};