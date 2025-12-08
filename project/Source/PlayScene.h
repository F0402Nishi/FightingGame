#pragma once
#include "../Library/SceneBase.h"
#include "Fade.h"

class Character;
class Player;
class CPU;
class UI2D;
class MiniWindow;

enum class Result {
	None,
	Win,
	Lose,
	Draw,
	P1Win,
	P2Win,
};

class PlayScene : public SceneBase
{
public:
	PlayScene();
	~PlayScene();
	void Update() override;
	void Draw() override;

/// <summary>
/// プレイヤー間の距離に応じてカメラ位置とズームを調整する。
/// </summary>
	void UpdateCamera();

/// <summary>
/// バトルのフェーズ管理とUI表示、勝敗判定を行う。
/// </summary>
	void UpdateBattleFont();

/// <summary>
/// バトル中のメニュー開閉と操作を管理する。
/// </summary>
	void MenuKey();

	static PlayScene* instance;
	static Result lastResult;
	Result GetResultType() const { return resultType; }
	// int GetType() const { return opponentType; }

private:
	Character* p1;
	Character* p2;
	CPU* cpu;
	UI2D* h1;
	UI2D* h2;
	UI2D* ui2d;
	Fade* fade;
	MiniWindow* miniwindow;
	Result resultType;

	int battlePhase; // 0=準備, 1=Ready表示中, 2=Fight表示中, 3=バトル中, 4=勝敗表示中
	int opponentType;
	int BattleBgm;
	int ReadySe;
	int KoSe;
	int option;

	bool PlayNow; // 勝敗を表示したい時用
	bool isMenu;
	bool isWind;
	bool openwind;
	bool wasStartPressed;
	bool startPressed;
	bool startHit;
	bool BgmStat;
};
