#pragma once
#include <string>
#include "Player.h"
#include "Object3D.h"

class HitCheck : public Object3D
{
public:
	std::string CheckHitToPart(const Player& target, const VECTOR& attackPos, float attackRadius);
};
