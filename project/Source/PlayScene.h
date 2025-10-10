#pragma once
#include "../Library/SceneBase.h"
#include "Fade.h"

class Character;
class Player;
class CPU;
class UI2D;
class MiniWindow;

class PlayScene : public SceneBase
{
public:
	PlayScene();
	~PlayScene();
	void Update() override;
	void Draw() override;
	void UpdateCamera();
	void UpdateBattleFont();
	void MenuKey();

	// int GetType() const { return opponentType; }

private:
	Character* p1;
	Character* p2;
	CPU* cpu;
	UI2D* h1;
	UI2D* h2;
	Fade* fade;
	MiniWindow* miniwindow;

	int battlePhase; // 0=準備, 1=Ready表示中, 2=Fight表示中, 3=バトル中, 4=勝敗表示中
	int opponentType;
	// int Hpbar;

	bool PlayNow; // 勝敗を表示したい時用
	bool isMenu;
	bool isWind;
	bool openwind;
};
