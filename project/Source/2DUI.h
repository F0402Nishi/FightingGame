#pragma once
#include <DxLib.h>
#include <algorithm>
#include <vector> 
#include "Object3D.h"

class Character;
class Player;
class CPU;
class MiniWindow;

enum class Battle {
	None,
	Training,
	Ready,
	Fight,
	KO,
};

enum class UIType {
	HP, // HPバー表示用
	B_Font, // メニュー表示用
};

class UI2D : public Object3D
{
public:

	UI2D();
	bool Init(Character* target, int Type, UIType _uitype);
	void Update() override;
	void Draw() override;
	void SetHp(int hp);
	void SetMaxHp(int hp);
	void HPbar();
	void SetMessage(Battle newBattle, int BattleFrame);
	void GameFont();
	void NameFont();
	void AttackInformation();
	void DrawInputHistory(const std::vector<int>& inputHistory);

	bool IsBattleFinish() const { return (battle == Battle::None); }

private:
	Character* character;
	Player* player;
	CPU* Cenemy;
	MiniWindow* mini;

	Battle battle; // 今表示しているメッセージ
	UIType uitype;

	VECTOR Ppos;
	VECTOR Cpos;

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
	int recoveryFrame; // ダメージ後何フレーム待つか
	int battleTime; // 表示してからの経過フレーム数 
	int battleFrame; // 何フレーム表示するか
	int gameType;
	int damageHp; // 受けたダメージ

	float angle; // 回転角度を保持する変数

	bool isLeftPlayer;
	bool isRightCpu;
};
