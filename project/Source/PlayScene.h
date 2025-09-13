#pragma once
#include "../Library/SceneBase.h"

class Player;
class Character;
class HPber;

class PlayScene : public SceneBase
{
public:
	PlayScene();
	~PlayScene();
	void Update() override;
	void Draw() override;
	void UpdateCamera();
	void InitGameFromSelect(int opponentType);

private:
	Player* p1;
	Player* p2;
	HPber* h1;
	HPber* h2;
	Character* cpu;

	int Hpbar;
	bool PlayerKeyInput;
};
