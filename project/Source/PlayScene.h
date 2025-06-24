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

private:
	Player* player;
};
