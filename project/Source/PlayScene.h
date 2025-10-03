#pragma once
#include "../Library/SceneBase.h"

class Character;
class Player;
class CPU;
class UI2D;
class Fade;

class PlayScene : public SceneBase
{
public:
	PlayScene();
	~PlayScene();
	void Update() override;
	void Draw() override;
	void UpdateCamera();

	int GetType() const { return opponentType; }

private:
	Character* p1;
	Character* p2;
	CPU* cpu;
	UI2D* h1;
	UI2D* h2;
	Fade* fade;

	int Hpbar;
	int opponentType;

	bool PlayerKeyInput;
};
