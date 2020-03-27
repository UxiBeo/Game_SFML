#pragma once
#include <entt/core/hashed_string.hpp>
namespace Database
{
	constexpr auto GridMap = "Data\\Json\\map200x200.tmx"_hs;
	constexpr auto TEnemy01 = "Data\\Images\\enemy_01.png"_hs;
	constexpr auto TEnemy02 = "Data\\Images\\enemy_02.png"_hs;
	constexpr auto TEnemy03 = "Data\\Images\\enemy_03.png"_hs;
	constexpr auto PlayerAnimation = "Data\\Json\\playerAnimation.json"_hs;
	constexpr auto BulletAnimation = "Data\\Json\\bulletAnimation.json"_hs;
	constexpr auto FontSplatch = "Data\\Fonts\\Adventure.otf"_hs;
	constexpr auto UpdateAnimation = "UpdateAnimation"_hs;
	constexpr auto Viewable = "Viewable"_hs;
	constexpr auto DestroyMe = "Destroy Me"_hs;
	/*------------------Attribute Stat-----------------------*/
	constexpr auto AHealth = "Att Health"_hs;
	constexpr auto AMana = "Att Mana"_hs;

	constexpr auto ADefence = "Att Defence"_hs;
	constexpr auto ADamage = "Att Damage"_hs;

	constexpr auto AFireRes = "Att Fire Resistance"_hs;
	constexpr auto AWaterRes = "Att Water Resistance"_hs;
	constexpr auto AFireMagic = "Att Fire Magic"_hs;
	constexpr auto AWaterMagic = "Att Water Magic"_hs;

	constexpr auto AIceRes = "Att Ice Resistance"_hs;
	constexpr auto ALightingRes = "Att Lighting Resistance"_hs;
	constexpr auto AIceMag = "Att Ice Magic"_hs;
	constexpr auto ALightingMag = "Att Lighting Magic"_hs;

	constexpr auto ANatureRes = "Att Nature Resistance"_hs;
	constexpr auto ADarkRes = "Att Dark Resistance"_hs;
	constexpr auto ANatureMag = "Att Nature Magic"_hs;
	constexpr auto ADarkMag = "Att Dark Magic"_hs;

	constexpr auto AMoveSpd = "Att Moving Speed"_hs;
	constexpr auto ACooldown = "Att Cooldown Reduction"_hs;
	/*------------------Attribute Stat-----------------------*/


	/*---------------Gameplay Tag----------------------------*/
}