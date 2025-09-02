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

private:
	Player* p1;
	Player* p2;
	HPber* h1;
	HPber* h2;

	int Hpbar;
	bool PlayerKeyInput;
};
