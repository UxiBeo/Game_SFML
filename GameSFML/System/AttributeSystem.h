#pragma once
#include "../System/ISystemECS.h"
#include "../Component/AttributeComponent.h"
#include <algorithm>
class AttributeSystem final : public ISystemECS
{
private:
	void Update(entt::registry& ECS) final;
	void BeginPlay(entt::registry& ECS) final;
public:
	template<typename EType, typename ValueType, typename ModifiedData>
	void ModifiedStat(entt::registry& ECS) const
	{
		auto group = ECS.group<RPGS::SBase<EType, ValueType>, const RPGS::SModified<EType, ModifiedData>>();
		std::for_each(std::execution::par_unseq, group.begin(), group.end(), [&group](auto entity) {

			auto [base, modified] = group.get<RPGS::SBase<EType, ValueType>, const RPGS::SModified<EType, ModifiedData>>(entity);
			std::transform(std::execution::par_unseq, base.data.begin(), base.data.end(), modified.data.begin(), base.data.begin(),
				[](auto& lhs, const auto& rhs) {return lhs += rhs; });
			});
	}
	template<typename EType, typename ModifiedData>
	void ClearModified(entt::registry& ECS) const
	{
		ECS.clear<RPGS::SModified<EType, ModifiedData>>();
	}
};

