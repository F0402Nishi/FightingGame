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

/// <summary>
/// 勝敗結果に応じてスケールや状態を更新する。
/// </summary>
	void UpdateResultFont();

/// <summary>
/// 勝敗結果に応じてリザルト画面の文字と画像を描画する。
/// </summary>
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
	bool playedWinSe;
	bool playedLoseSe;
};
