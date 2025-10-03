#pragma once
#include <DxLib.h>
#include <algorithm>
#include "Object3D.h"
// #include "Player.h"

class Character;

class UI2D : public Object3D
{
public:
	UI2D();
	bool Init(Character* target);
	void Update() override;
	void Draw() override;
	void SetHp(int hp);
	void SetMaxHp(int hp);
	void HPbar();

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
	int blueY;  // 画面上端からスタート
	int blueSpeed; // 降下速度
	int fullTank;
	int recoveryDelayTimer; // ダメージ後の待機時間カウント

	float angle; // 回転角度を保持する変数

	bool isLeftPlayer;
	bool isRightCpu;

#if false
	int HPbarCount = 4;
	int HPbarHeight = 340 / HPbarCount;
	int HPbarWidth = 453;
	int maxHP = 1000;
	int currentHP = 1000;
#endif
};
