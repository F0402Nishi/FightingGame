#pragma once
#include <string>
#include "Character.h"
#include "Object3D.h"
#include <DxLib.h>

class HitCheck : public Object3D
{
public:
	static std::string CheckHitToPart(const Character& target, const VECTOR& attackPos, float attackRadius);
};
