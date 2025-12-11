#pragma once
#include "../Library/SceneBase.h"
#include "Screen.h"
#include "SoundManager.h"


/// <summary>
/// タイトルシーン
/// </summary>
class TitleScene : public SceneBase
{
public:
	TitleScene();
	~TitleScene();
	void Update() override;
	void Draw() override;

private:
	int MechImage;
	int BrawlersImage;
	int TitleBackImage;

	float Mechscale; // 開始時の縮小率
	float Brawlersscale; // 開始時の縮小率
	float scaleSpeed; // 拡大スピード
	float statFontScale;

	bool ScaleCount;
	bool playSe;
	bool playedSecond;
	bool scaleOk;
};
