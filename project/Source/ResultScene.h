#pragma once
#include "../Library/SceneBase.h"
#include "PlayScene.h"
#include "Fade.h"

class ResultScene : public SceneBase
{
public:
	ResultScene();
	~ResultScene();
	void Update() override;
	void Draw() override;

private:
	PlayScene* playscene;
	Fade* fade;
	Result resultNumber;

	int resultBackImage;
};
