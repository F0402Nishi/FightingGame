#pragma once
#include <DxLib.h>
#include <algorithm>
#include "Object3D.h"
// #include "Player.h"

class Character;

enum class Battle {
	None,
	Ready,
	Fight,
	Win,
	Lose,
};

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
	void SetMessage(Battle newBattle, int BattleFrame);


private:
	Character* character;

	Battle battle; // 今表示しているメッセージ

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
	int recoveryTimer; // ダメージ後の待機時間カウント
	int recoveryHp; // まだ回復すべき残りHP（ダメージ量）
	// int recoverySpeed = 1; // 1フレームあたりの回復量（調整用）
	int recoveryFrame; // ダメージ後何フレーム待つか
	int battleTime; // 表示してからの経過フレーム数 
	int battleFrame; // 何フレーム表示するか

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
