#include "..\..\System\ParentChildrenSystem.h"
#include "../../Component/ParentChildrenComponent.h"

void ParentChildrenSystem::Update(entt::registry& ECS)const
{
	
}

void ParentChildrenSystem::BeginPlay(entt::registry& ECS)const
{
	ECS.on_destroy<ParentComponent>().connect<&ParentChildrenSystem::DestroyParentComponent>();
}

void ParentChildrenSystem::DestroyParentComponent(entt::registry& ECS, entt::entity entity)
{
	const auto& ens = ECS.get<ParentComponent>(entity).childEntities;
	ECS.destroy(ens.begin(), ens.end());
}
