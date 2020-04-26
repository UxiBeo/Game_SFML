#pragma once
#include "../Component/GameplayTag.h"
struct TagRule
{
	Tag::Bitfiled blockMoveTags = 1 | (1 << 2);
	Tag::Bitfiled attacking = 1;
};