#pragma once
#include "../Library/SceneBase.h"

class Character;
class Player;
class CPU;
class HPber;

class PlayScene : public SceneBase
{
public:
	PlayScene();
	~PlayScene();
	void Update() override;
	void Draw() override;
	void UpdateCamera();

private:
	Character* p1;
	Character* p2;
	CPU* cpu;
	HPber* h1;
	HPber* h2;

	int Hpbar;
	int opponentType;

	bool PlayerKeyInput;
};
