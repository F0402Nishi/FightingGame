#pragma once
#include <DxLib.h>
#include <algorithm>
#include "Object3D.h"
#include "Player.h"

class Character;

class HPber : public Object3D
{
public:
	HPber();
	bool Init(Player* target);
	void Update() override;
	void Draw() override;
	void SetHp(int hp);
	void SetMaxHp(int hp);

private:
	Player* player;
	int HPbackImage = -1;
	int HPImage = -1;
	int currenthp;
	int maxhp;
	int graphW;
	int graphH;
	int displayHp;
	bool isLeftPlayer;

#if false
	int HPbarCount = 4;
	int HPbarHeight = 340 / HPbarCount;
	int HPbarWidth = 453;
	int maxHP = 1000;
	int currentHP = 1000;
#endif
};
