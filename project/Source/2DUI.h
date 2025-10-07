#pragma once
#include <DxLib.h>
#include <algorithm>
#include "Object3D.h"
// #include "Player.h"

class Character;
class MiniWindow;

enum class Battle {
	None,
	Training,
	Ready,
	Fight,
	Win,
	Lose,
	Draw,
};

enum class UIType {
	HP, // HPバー表示用
	MENU, // メニュー表示用
};

class UI2D : public Object3D
{
public:

	UI2D();
	bool Init(Character* target, int Type, UIType _type);
	void Update() override;
	void Draw() override;
	void SetHp(int hp);
	void SetMaxHp(int hp);
	void HPbar();
	void SetMessage(Battle newBattle, int BattleFrame);
	void SetMenu(bool _menu);

	bool IsBattleFinish() const { return (battle == Battle::None); }
	bool IsMenuOpen() const { return MenuOpen; }


private:
	Character* character;
	MiniWindow* miniwindow;

	Battle battle; // 今表示しているメッセージ
	UIType uitype;

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
	int gameType;

	float angle; // 回転角度を保持する変数

	bool isLeftPlayer;
	bool isRightCpu;
	bool MenuOpen;

#if false
	int HPbarCount = 4;
	int HPbarHeight = 340 / HPbarCount;
	int HPbarWidth = 453;
	int maxHP = 1000;
	int currentHP = 1000;
#endif
};
