#include "..\..\System\ParentChildrenSystem.h"
#include "../../Component/ParentChildrenComponent.h"

void ParentChildrenSystem::Update(entt::registry& ECS)
{
	ECS.group<RemoveChildrent, ParentComponent>().each([](auto entity, const auto& remove, auto& parent) {
		for (auto s : remove.entties)
		{
			parent.childEntities.erase(std::remove_if(parent.childEntities.begin(), parent.childEntities.end(),
				[s](auto rhs) {return s == rhs; }), parent.childEntities.end());
		}
		});
	ECS.clear<RemoveChildrent>();
}

void ParentChildrenSystem::BeginPlay(entt::registry& ECS)
{
	ECS.on_destroy<ParentComponent>().connect<&ParentChildrenSystem::DestroyParentComponent>();
}

void ParentChildrenSystem::DestroyParentComponent(entt::registry& ECS, entt::entity entity)
{
	const auto& ens = ECS.get<ParentComponent>(entity).childEntities;
	ECS.destroy(ens.begin(), ens.end());
}
