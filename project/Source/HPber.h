#pragma once
#include <DxLib.h>
#include "Player.h"

class HPber
{
public:
	bool Init(const char* filename);
	void SetHp(int hp);
	void SetMaxHp(int hp);
	void Draw(int x, int y);

private:
	int HPImage = -1;
	int HPbarCount = 4;
	int HPbarHeight = 340 / HPbarCount;
	int HPbarWidth = 453;
	int maxHP = 1000;
	int currenHP = 1000;
};
