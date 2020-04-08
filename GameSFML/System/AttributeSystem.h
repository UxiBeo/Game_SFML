#pragma once
#include "../System/ISystemECS.h"
#include "../Component/AttributeComponent.h"
#include <algorithm>
class AttributeSystem final : public ISystemECS
{
	using ATT = RPGS::AttributeType;
private:
	void Update(entt::registry& ECS) final;
	void BeginPlay(entt::registry& ECS) final;
public:
	void ClearModified(entt::registry& ECS) const;
	template<RPGS::AttributeType T>
	void ModifiedAttribute(entt::registry& ECS) const
	{
		using MType = RPGS::ModifiedAttribute<T>;
		using AType = RPGS::Attribute<T>;
		auto view = ECS.view<const MType, AType>();
		std::for_each(std::execution::par_unseq, view.begin(), view.end(), [&view](auto entity) {
			auto [modified, value] = view.get<const MType, AType>(entity);
			value.value += modified.value;
			});
	}
};

