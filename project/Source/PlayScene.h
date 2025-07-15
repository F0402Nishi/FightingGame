#pragma once
#include "../Library/SceneBase.h"
#include "Player.h"
#include "HPber.h"

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

	int Hpbar;
	bool PlayerKeyInput;
};
