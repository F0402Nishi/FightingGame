#pragma once
#include "../Library/SceneBase.h"
#include "Player.h"

class PlayScene : public SceneBase
{
public:
	PlayScene();
	~PlayScene();
	void Update() override;
	void Draw() override;
	void UpdateCamera();
	void DrawHpBer(int x, int y, int hp, int maxhp, int hpImage);

private:
	Player* p1;
	Player* p2;
	int Hpbar;
	bool PlayerKeyInput;
	int HPImage;
};
