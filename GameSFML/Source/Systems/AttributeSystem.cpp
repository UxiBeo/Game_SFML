#define AttXMacro
#include "../../System/AttributeSystem.h"
#include <future>
#include <vector>
void AttributeSystem::Update(entt::registry& ECS)
{
	std::vector<std::future<void>> tasks;
	#define X(el) tasks.emplace_back(std::async(std::launch::async,\
	 &AttributeSystem::ModifiedAttribute<ATT::el>, this, std::ref(ECS)));
		LEAF_ELEMENT_TYPES
	#undef X
	
	for (auto& t : tasks)
	{
		t.wait();
	}
}

void AttributeSystem::BeginPlay(entt::registry& ECS)
{
	
}

void AttributeSystem::ClearModified(entt::registry& ECS) const
{
	#define X(el) ECS.clear<RPGS::ModifiedAttribute<ATT::el>>();
	LEAF_ELEMENT_TYPES
	#undef X
}			 
