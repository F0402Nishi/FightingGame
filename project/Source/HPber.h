#pragma once
#include <DxLib.h>
#include <algorithm>
#include "Object3D.h"
// #include "Player.h"

class Character;

class HPber : public Object3D
{
public:
	HPber();
	bool Init(Character* target);
	void Update() override;
	void Draw() override;
	void SetHp(int hp);
	void SetMaxHp(int hp);

private:
	Character* character;
	int HPbackImage = -1;
	int HPImage = -1;
	int HPImageLeft = -1;
	int currenthp;
	int maxhp;
	int graphW;
	int graphH;
	int displayHp;
	int WinImage;
	int blackoutAlpha; // 透明度（0=透明, 255=真っ黒）

	
	float angle; // 回転角度を保持する変数

	bool isLeftPlayer;
	bool isRightCpu;
	bool blackoutActive; // フェードアウトを有効にする

#if false
	int HPbarCount = 4;
	int HPbarHeight = 340 / HPbarCount;
	int HPbarWidth = 453;
	int maxHP = 1000;
	int currentHP = 1000;
#endif
};
