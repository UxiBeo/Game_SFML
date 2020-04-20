#pragma once
#include "../System/ISystemECS.h"
#include "../Component/AttributeComponent.h"
#include <algorithm>
class AttributeSystem final : public ISystemECS
{
private:
	void Update(entt::registry& ECS) const final;
public:
	void ApplyDame(entt::registry& ECS) const;
};

