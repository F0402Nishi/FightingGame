#pragma once
#include "../Library/SceneBase.h"
#include "PlayScene.h"
#include "Fade.h"
#include "MiniWindow.h"
#include "SoundManager.h"

/// <summary>
/// リザルトシーン
/// </summary>
class ResultScene : public SceneBase
{
public:
	ResultScene();
	~ResultScene();
	void Update() override;
	void Draw() override;
	void UpdateResultFont();
	void DrawResultFont();

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

	float winScale;   // 勝者：最初は小さく
	float loseScale;  // 敗者：後から出てくる

	bool windowClose;
	bool winFinished;
};
