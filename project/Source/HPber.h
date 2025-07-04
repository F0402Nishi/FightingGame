#pragma once
#include <DxLib.h>
#include "Object3D.h"
#include "Player.h"

class HPber : public Object3D
{
public:
	HPber();
	bool Init(const char* filename);
	void SetHp(int hp);
	void SetMaxHp(int hp);
	void Draw() override;

private:
	int HPImage = -1;
	int HPbarCount = 4;
	int HPbarHeight = 340 / HPbarCount;
	int HPbarWidth = 453;
	int maxHP = 1000;
	int currenHP = 1000;
};
