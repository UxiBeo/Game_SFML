#pragma once
#include <entt/entt.hpp>

class StampContex
{
	using stamp_fn_type = void(const entt::registry&, const entt::entity, entt::registry&, const entt::entity);
	template<typename Type>
	static void stamp(const entt::registry& from, const entt::entity src, entt::registry& to, const entt::entity dst) {
		to.assign_or_replace<Type>(dst, from.get<Type>(src));
	}
public:
	template<typename Type>
	void AddStampFunction()
	{
		stamp_functions[entt::type_info<Type>::id()] = &stamp<Type>;
	}
	void Clone(const entt::registry& from, const entt::entity src, entt::registry& to, const entt::entity dst)
	{
		from.visit(src, [this, &from, &to, dst, src](const auto type_id) {
			stamp_functions[type_id](from, src, to, dst);
			});
	}
private:
	std::unordered_map<uint32_t, stamp_fn_type*> stamp_functions;
};