#pragma once
#include "../Library/SceneBase.h"
#include "PlayScene.h"
#include "Fade.h"
#include "MiniWindow.h"

class ResultScene : public SceneBase
{
public:
	ResultScene();
	~ResultScene();
	void Update() override;
	void Draw() override;
	void UpdateResultFont();

private:
	PlayScene* playscene;
	Fade* fade;
	Result resultNumber;
	MiniWindow* miniwindow;

	int resultBackImage;
	int winImage;
	int loseImage;
	int resultnumbers;
	int sceneNumber;
};
